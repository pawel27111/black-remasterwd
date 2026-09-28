#include "BlackRemasteredInteractionComponent.h"
#include "BlackRemasteredCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UBlackRemasteredInteractionComponent::UBlackRemasteredInteractionComponent()
    : Super()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Initialize state
    OwnerCharacter = nullptr;
    CurrentInteractable = nullptr;
    CurrentInteractionState = IS_None;
    InteractionProgress = 0.0f;
    
    // Settings
    DetectionRadius = 200.0f;
    DetectionAngle = 60.0f;
    DetectionFrequency = 0.1f;
    bRequireLineOfSight = true;
}

void UBlackRemasteredInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Get owner character
    if (AActor* Owner = GetOwner())
    {
        OwnerCharacter = Cast<ABlackRemasteredCharacter>(Owner);
    }
    
    // Start detection timer
    GetWorld()->GetTimerManager().SetTimer(
        DetectionTimerHandle,
        this,
        &UBlackRemasteredInteractionComponent::OnDetectionTimer,
        DetectionFrequency,
        true
    );
}

void UBlackRemasteredInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Update interaction progress
    if (CurrentInteractionState == IS_Interacting)
    {
        InteractionProgress += DeltaTime / CurrentInteractionDuration;
        
        if (InteractionProgress >= 1.0f)
        {
            CompleteInteraction();
        }
    }
}

void UBlackRemasteredInteractionComponent::Initialize(ABlackRemasteredCharacter* Owner)
{
    OwnerCharacter = Owner;
}

void UBlackRemasteredInteractionComponent::Interact()
{
    if (!CanInteract())
    {
        return;
    }
    
    if (CurrentInteractable)
    {
        InteractWithActor(CurrentInteractable);
    }
    else if (HasInteractables())
    {
        // Interact with closest interactable
        InteractWithActor(Interactables[0]);
    }
}

void UBlackRemasteredInteractionComponent::InteractWithActor(AActor* Actor)
{
    if (!Actor || !CanInteract())
    {
        return;
    }
    
    // Check if actor is valid and interactable
    if (!IsValidInteractable(Actor))
    {
        return;
    }
    
    // Check line of sight if required
    if (bRequireLineOfSight && !HasLineOfSight(Actor))
    {
        return;
    }
    
    // Start interaction
    StartInteraction(Actor);
}

void UBlackRemasteredInteractionComponent::CancelInteraction()
{
    if (CurrentInteractionState != IS_Interacting)
    {
        return;
    }
    
    // Check if we can cancel
    FInteractionData InteractionData = GetInteractionData(CurrentInteractable);
    if (!InteractionData.bCanCancel)
    {
        return;
    }
    
    // Cancel interaction
    CurrentInteractionState = IS_None;
    InteractionProgress = 0.0f;
    
    // Clear timers
    GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
    
    // Broadcast cancelled event
    OnInteractionCancelled.Broadcast(CurrentInteractable);
    
    // Clear current interactable
    CurrentInteractable = nullptr;
}

void UBlackRemasteredInteractionComponent::DetectInteractables()
{
    // Clear current interactables
    ClearInteractables();
    
    // Find new interactables
    FindInteractables();
    
    // Sort by distance
    SortInteractables();
    
    // Select best interactable
    SelectBestInteractable();
}

void UBlackRemasteredInteractionComponent::ClearInteractables()
{
    // Clear all interactables
    TArray<AActor*> OldInteractables = Interactables;
    Interactables.Empty();
    
    // Notify lost interactables
    for (AActor* Actor : OldInteractables)
    {
        if (Actor == CurrentInteractable)
        {
            CurrentInteractable = nullptr;
            CurrentInteractionState = IS_None;
        }
        
        OnInteractableLost.Broadcast(Actor);
    }
}

void UBlackRemasteredInteractionComponent::FindInteractables()
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Get all actors in detection radius
    TArray<AActor*> AllActors;
    
    // Overlap sphere to find actors
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerCharacter);
    
    GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        OwnerCharacter->GetActorLocation(),
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(DetectionRadius),
        QueryParams
    );
    
    // Filter and add valid interactables
    for (FOverlapResult& OverlapResult : OverlapResults)
    {
        if (AActor* Actor = OverlapResult.GetActor())
        {
            if (IsValidInteractable(Actor))
            {
                // Check angle
                FVector DirectionToActor = (Actor->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
                FVector OwnerForward = OwnerCharacter->GetActorForwardVector();
                
                float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(DirectionToActor, OwnerForward)));
                
                if (Angle <= DetectionAngle * 0.5f)
                {
                    // Check line of sight if required
                    if (!bRequireLineOfSight || HasLineOfSight(Actor))
                    {
                        Interactables.AddUnique(Actor);
                    }
                }
            }
        }
    }
}

void UBlackRemasteredInteractionComponent::SortInteractables()
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Sort interactables by distance to character
    Interactables.Sort([this](const AActor& A, const AActor& B) {
        if (!OwnerCharacter)
        {
            return false;
        }
        
        float DistanceA = FVector::Distance(OwnerCharacter->GetActorLocation(), A.GetActorLocation());
        float DistanceB = FVector::Distance(OwnerCharacter->GetActorLocation(), B.GetActorLocation());
        
        return DistanceA < DistanceB;
    });
}

void UBlackRemasteredInteractionComponent::SelectBestInteractable()
{
    if (Interactables.Num() == 0)
    {
        if (CurrentInteractable)
        {
            OnInteractableLost.Broadcast(CurrentInteractable);
            CurrentInteractable = nullptr;
        }
        return;
    }
    
    // Select closest interactable
    AActor* NewInteractable = Interactables[0];
    
    if (NewInteractable != CurrentInteractable)
    {
        // Lost old interactable
        if (CurrentInteractable)
        {
            OnInteractableLost.Broadcast(CurrentInteractable);
        }
        
        // Found new interactable
        CurrentInteractable = NewInteractable;
        CurrentInteractionState = IS_Found;
        InteractionProgress = 0.0f;
        
        OnInteractableFound.Broadcast(CurrentInteractable);
    }
}

bool UBlackRemasteredInteractionComponent::IsValidInteractable(AActor* Actor) const
{
    if (!Actor || Actor->IsPendingKill())
    {
        return false;
    }
    
    // Check if actor has the interactable interface or tag
    // For now, we'll assume all actors are potentially interactable
    // In a real implementation, you would check for a specific interface or tag
    
    return true;
}

bool UBlackRemasteredInteractionComponent::HasLineOfSight(AActor* Actor) const
{
    if (!OwnerCharacter || !Actor)
    {
        return false;
    }
    
    // Trace from character to actor
    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(OwnerCharacter);
    TraceParams.AddIgnoredActor(Actor);
    
    FVector StartLocation = OwnerCharacter->GetActorLocation();
    StartLocation.Z += 50.0f; // Raise to eye level
    
    FVector EndLocation = Actor->GetActorLocation();
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        TraceParams
    );
    
    // If we hit something, check if it's the target actor
    if (bHit)
    {
        return HitResult.GetActor() == Actor;
    }
    
    return true;
}

void UBlackRemasteredInteractionComponent::StartInteraction(AActor* Actor)
{
    if (!Actor || CurrentInteractionState == IS_Interacting)
    {
        return;
    }
    
    // Set current interactable
    CurrentInteractable = Actor;
    CurrentInteractionState = IS_Interacting;
    InteractionProgress = 0.0f;
    
    // Get interaction data
    FInteractionData Data = GetInteractionData(Actor);
    CurrentInteractionDuration = Data.InteractionDuration;
    
    // Start interaction timer
    if (CurrentInteractionDuration > 0)
    {
        GetWorld()->GetTimerManager().SetTimer(
            InteractionTimerHandle,
            this,
            &UBlackRemasteredInteractionComponent::OnInteractionTimer,
            CurrentInteractionDuration,
            false
        );
    }
    else
    {
        // Instant interaction
        CompleteInteraction();
    }
    
    // Broadcast started event
    OnInteractionStarted.Broadcast(Actor);
    
    // Set character state
    if (OwnerCharacter)
    {
        OwnerCharacter->SetCharacterState(CS_Interacting);
    }
}

void UBlackRemasteredInteractionComponent::CompleteInteraction()
{
    if (CurrentInteractionState != IS_Interacting)
    {
        return;
    }
    
    // Set state
    CurrentInteractionState = IS_Cooldown;
    InteractionProgress = 1.0f;
    
    // Clear timers
    GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
    
    // Broadcast completed event
    OnInteractionCompleted.Broadcast(CurrentInteractable);
    
    // Perform interaction on actor
    // This would typically call a function on the interactable actor
    // For now, we'll just log it
    
    // Start cooldown
    FInteractionData Data = GetInteractionData(CurrentInteractable);
    if (Data.CooldownDuration > 0)
    {
        GetWorld()->GetTimerManager().SetTimer(
            CooldownTimerHandle,
            this,
            &UBlackRemasteredInteractionComponent::OnCooldownTimer,
            Data.CooldownDuration,
            false
        );
    }
    else
    {
        CurrentInteractionState = IS_None;
    }
    
    // Reset character state
    if (OwnerCharacter)
    {
        OwnerCharacter->SetCharacterState(CS_Idle);
    }
}

FText UBlackRemasteredInteractionComponent::GetInteractionDisplayText() const
{
    if (CurrentInteractable)
    {
        FInteractionData Data = GetInteractionData(CurrentInteractable);
        if (!Data.DisplayText.IsEmpty())
        {
            return Data.DisplayText;
        }
    }
    
    return FText::FromString("Interact");
}

bool UBlackRemasteredInteractionComponent::CanInteract() const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    // Check if character is in a valid state
    ECharacterState CharacterState = OwnerCharacter->GetCharacterState();
    
    switch (CharacterState)
    {
        case CS_Dead:
        case CS_Interacting:
        case CS_Mantling:
        case CS_Sliding:
            return false;
            
        default:
            break;
    }
    
    // Check if we have an interactable
    if (CurrentInteractionState == IS_None && !HasInteractables())
    {
        return false;
    }
    
    return true;
}

// Timer callbacks
void UBlackRemasteredInteractionComponent::OnDetectionTimer()
{
    DetectInteractables();
}

void UBlackRemasteredInteractionComponent::OnInteractionTimer()
{
    CompleteInteraction();
}

void UBlackRemasteredInteractionComponent::OnCooldownTimer()
{
    CurrentInteractionState = IS_None;
    InteractionProgress = 0.0f;
}

FInteractionData UBlackRemasteredInteractionComponent::GetInteractionData(AActor* Actor) const
{
    FInteractionData DefaultData;
    DefaultData.InteractionName = FName("Default");
    DefaultData.InteractionType = IT_Use;
    DefaultData.InteractionDistance = 200.0f;
    DefaultData.InteractionDuration = 0.5f;
    DefaultData.CooldownDuration = 0.0f;
    DefaultData.bRequireLineOfSight = true;
    DefaultData.bCanCancel = true;
    DefaultData.DisplayText = FText::FromString("Use");
    DefaultData.ActionName = FName("Use");
    
    // In a real implementation, you would get the interaction data from the actor
    // For now, we'll return default data
    
    return DefaultData;
}
