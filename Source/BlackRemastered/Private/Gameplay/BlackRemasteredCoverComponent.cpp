#include "BlackRemasteredCoverComponent.h"
#include "BlackRemasteredCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

UBlackRemasteredCoverComponent::UBlackRemasteredCoverComponent()
    : Super()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Initialize state
    OwnerCharacter = nullptr;
    bIsInCover = false;
    CurrentCoverSide = CS_None;
    CurrentCoverType = CT_None;
    CurrentCoverHeight = 0.0f;
    
    // Settings
    CoverDetectionRadius = 200.0f;
    CoverDetectionAngle = 120.0f;
    CoverSnapDistance = 50.0f;
    CoverHeightThreshold = 100.0f;
    CoverWidthThreshold = 50.0f;
}

void UBlackRemasteredCoverComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Get owner character
    if (AActor* Owner = GetOwner())
    {
        OwnerCharacter = Cast<ABlackRemasteredCharacter>(Owner);
    }
    
    // Start cover check timer
    GetWorld()->GetTimerManager().SetTimer(
        CoverCheckTimerHandle,
        this,
        &UBlackRemasteredCoverComponent::OnCoverCheckTimer,
        0.1f,
        true
    );
}

void UBlackRemasteredCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Update cover state
    UpdateCoverState();
}

void UBlackRemasteredCoverComponent::Initialize(ABlackRemasteredCharacter* Owner)
{
    OwnerCharacter = Owner;
}

void UBlackRemasteredCoverComponent::CheckForCover()
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Check if we're already in cover
    if (bIsInCover)
    {
        return;
    }
    
    // Find cover
    FCoverData NewCoverData;
    if (FindCover(NewCoverData))
    {
        // Found cover, enter it
        CurrentCoverData = NewCoverData;
        EnterCover();
    }
}

void UBlackRemasteredCoverComponent::UpdateCoverState()
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Check if we're still in cover
    if (bIsInCover)
    {
        // Check if cover is still valid
        FCoverData NewCoverData;
        if (!FindCover(NewCoverData) || NewCoverData.CoverActor != CurrentCoverData.CoverActor)
        {
            // Cover is no longer valid, exit
            ExitCover();
        }
        else
        {
            // Update cover data
            CurrentCoverData = NewCoverData;
            UpdateCoverType();
        }
    }
    else
    {
        // Check for new cover
        CheckForCover();
    }
}

void UBlackRemasteredCoverComponent::EnterCover()
{
    if (bIsInCover)
    {
        return;
    }
    
    bIsInCover = true;
    
    // Update cover type
    UpdateCoverType();
    
    // Apply cover position
    ApplyCoverPosition();
    
    // Apply cover rotation
    ApplyCoverRotation();
    
    // Set character state
    if (OwnerCharacter)
    {
        OwnerCharacter->SetCharacterState(CS_InCover);
    }
    
    // Broadcast cover entered
    OnCoverEntered.Broadcast();
}

void UBlackRemasteredCoverComponent::ExitCover()
{
    if (!bIsInCover)
    {
        return;
    }
    
    bIsInCover = false;
    CurrentCoverSide = CS_None;
    CurrentCoverType = CT_None;
    CurrentCoverHeight = 0.0f;
    CurrentCoverData = FCoverData();
    
    // Reset character state
    if (OwnerCharacter)
    {
        OwnerCharacter->SetCharacterState(CS_Idle);
    }
    
    // Broadcast cover exited
    OnCoverExited.Broadcast();
}

void UBlackRemasteredCoverComponent::ToggleCover()
{
    if (bIsInCover)
    {
        ExitCover();
    }
    else
    {
        CheckForCover();
    }
}

void UBlackRemasteredCoverComponent::StartLeanLeft()
{
    if (!CanLeanLeft())
    {
        return;
    }
    
    if (OwnerCharacter)
    {
        OwnerCharacter->SetLeanDirection(LD_Left);
    }
}

void UBlackRemasteredCoverComponent::StartLeanRight()
{
    if (!CanLeanRight())
    {
        return;
    }
    
    if (OwnerCharacter)
    {
        OwnerCharacter->SetLeanDirection(LD_Right);
    }
}

void UBlackRemasteredCoverComponent::StopLean()
{
    if (OwnerCharacter)
    {
        OwnerCharacter->SetLeanDirection(LD_None);
    }
}

bool UBlackRemasteredCoverComponent::CanLean() const
{
    if (!bIsInCover)
    {
        return false;
    }
    
    return CanLeanLeft() || CanLeanRight();
}

bool UBlackRemasteredCoverComponent::CanLeanLeft() const
{
    if (!bIsInCover)
    {
        return false;
    }
    
    return CurrentCoverData.bCanLeanLeft;
}

bool UBlackRemasteredCoverComponent::CanLeanRight() const
{
    if (!bIsInCover)
    {
        return false;
    }
    
    return CurrentCoverData.bCanLeanRight;
}

bool UBlackRemasteredCoverComponent::FindCover(FCoverData& OutCoverData) const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    // Check for cover in multiple directions
    FVector CharacterLocation = OwnerCharacter->GetActorLocation();
    FVector CharacterForward = OwnerCharacter->GetActorForwardVector();
    
    // Check front
    if (CheckCoverInDirection(CharacterLocation, CharacterForward, OutCoverData))
    {
        OutCoverData.CoverSide = CS_Front;
        return true;
    }
    
    // Check left
    FVector LeftDirection = FVector::CrossProduct(CharacterForward, FVector::UpVector).GetSafeNormal();
    if (CheckCoverInDirection(CharacterLocation, LeftDirection, OutCoverData))
    {
        OutCoverData.CoverSide = CS_Left;
        return true;
    }
    
    // Check right
    FVector RightDirection = FVector::CrossProduct(FVector::UpVector, CharacterForward).GetSafeNormal();
    if (CheckCoverInDirection(CharacterLocation, RightDirection, OutCoverData))
    {
        OutCoverData.CoverSide = CS_Right;
        return true;
    }
    
    // Check back
    FVector BackDirection = -CharacterForward;
    if (CheckCoverInDirection(CharacterLocation, BackDirection, OutCoverData))
    {
        OutCoverData.CoverSide = CS_Back;
        return true;
    }
    
    return false;
}

bool UBlackRemasteredCoverComponent::CheckCoverInDirection(FVector Location, FVector Direction, FCoverData& OutCoverData) const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    // Trace for cover in the specified direction
    FVector Start = Location;
    Start.Z += 50.0f; // Raise to eye level
    
    FVector End = Start + (Direction * CoverDetectionRadius);
    
    FHitResult HitResult;
    if (!TraceForCover(Start, End, HitResult))
    {
        return false;
    }
    
    // Check if this is valid cover
    if (!IsValidCover(HitResult.GetActor(), HitResult))
    {
        return false;
    }
    
    // Fill out cover data
    OutCoverData.CoverActor = HitResult.GetActor();
    OutCoverData.CoverLocation = HitResult.ImpactPoint;
    OutCoverData.CoverNormal = HitResult.Normal;
    
    return true;
}

bool UBlackRemasteredCoverComponent::IsValidCover(AActor* Actor, const FHitResult& HitResult) const
{
    if (!Actor)
    {
        return false;
    }
    
    // Check if the surface is vertical enough
    float SurfaceAngle = FMath::Abs(FMath::Asin(HitResult.Normal.Z));
    
    if (SurfaceAngle < 60.0f) // More than 60 degrees from horizontal (mostly vertical)
    {
        return false;
    }
    
    // Check if the cover is tall enough
    // Trace down from the impact point to find the bottom of the cover
    FVector TraceStart = HitResult.ImpactPoint;
    FVector TraceEnd = TraceStart;
    TraceEnd.Z -= 200.0f; // Trace down 200 units
    
    FHitResult BottomHit;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(Actor);
    
    bool bHitBottom = GetWorld()->LineTraceSingleByChannel(
        BottomHit,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        TraceParams
    );
    
    // Calculate cover height
    float CoverHeight = 0.0f;
    
    if (bHitBottom)
    {
        CoverHeight = FVector::Distance(TraceStart, BottomHit.ImpactPoint);
    }
    else
    {
        CoverHeight = FVector::Distance(TraceStart, TraceEnd);
    }
    
    // Check if cover is tall enough
    if (CoverHeight < CoverHeightThreshold * 0.5f)
    {
        return false;
    }
    
    return true;
}

void UBlackRemasteredCoverComponent::ApplyCoverPosition()
{
    if (!OwnerCharacter || !CurrentCoverData.CoverActor)
    {
        return;
    }
    
    // Calculate position relative to cover
    FVector CharacterLocation = OwnerCharacter->GetActorLocation();
    FVector CoverLocation = CurrentCoverData.CoverLocation;
    FVector CoverNormal = CurrentCoverData.CoverNormal;
    
    // Calculate distance to cover
    float DistanceToCover = FVector::Distance(CharacterLocation, CoverLocation);
    
    // Snap to cover if within snap distance
    if (DistanceToCover <= CoverSnapDistance)
    {
        // Calculate offset from cover based on character size
        float CharacterRadius = OwnerCharacter->GetCapsuleComponent() ? 
            OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() : 34.0f;
        
        // Position character at cover with offset
        FVector NewLocation = CoverLocation + (CoverNormal * (CharacterRadius + 10.0f));
        
        // Adjust height based on cover type
        switch (CurrentCoverType)
        {
            case CT_Low:
                // Crouch height
                NewLocation.Z = OwnerCharacter->GetActorLocation().Z;
                break;
                
            case CT_High:
            case CT_Full:
                // Stand height
                NewLocation.Z = OwnerCharacter->GetActorLocation().Z;
                break;
                
            default:
                break;
        }
        
        // Set character location
        OwnerCharacter->SetActorLocation(NewLocation);
    }
}

void UBlackRemasteredCoverComponent::ApplyCoverRotation()
{
    if (!OwnerCharacter || !CurrentCoverData.CoverActor)
    {
        return;
    }
    
    // Calculate rotation to face cover
    FVector CharacterLocation = OwnerCharacter->GetActorLocation();
    FVector CoverLocation = CurrentCoverData.CoverLocation;
    
    // Calculate direction to cover
    FVector DirectionToCover = (CoverLocation - CharacterLocation).GetSafeNormal();
    
    // Calculate yaw to face cover
    float Yaw = FMath::Atan2(DirectionToCover.Y, DirectionToCover.X) * (180.0f / PI);
    
    // Set character rotation
    FRotator NewRotation = OwnerCharacter->GetActorRotation();
    NewRotation.Yaw = Yaw;
    OwnerCharacter->SetActorRotation(NewRotation);
}

void UBlackRemasteredCoverComponent::UpdateCoverType()
{
    if (!CurrentCoverData.CoverActor)
    {
        CurrentCoverType = CT_None;
        CurrentCoverHeight = 0.0f;
        return;
    }
    
    // Determine cover type based on height
    // Trace down from the top of the cover to find the bottom
    FVector TraceStart = CurrentCoverData.CoverLocation;
    TraceStart.Z += 100.0f; // Start above cover
    
    FVector TraceEnd = TraceStart;
    TraceEnd.Z -= 300.0f; // Trace down 300 units
    
    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(CurrentCoverData.CoverActor);
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        TraceParams
    );
    
    // Calculate cover height
    CurrentCoverHeight = 0.0f;
    
    if (bHit)
    {
        CurrentCoverHeight = FVector::Distance(TraceStart, HitResult.ImpactPoint);
    }
    else
    {
        CurrentCoverHeight = FVector::Distance(TraceStart, TraceEnd);
    }
    
    // Determine cover type
    if (CurrentCoverHeight >= CoverHeightThreshold * 1.5f)
    {
        CurrentCoverType = CT_Full;
    }
    else if (CurrentCoverHeight >= CoverHeightThreshold)
    {
        CurrentCoverType = CT_High;
    }
    else
    {
        CurrentCoverType = CT_Low;
    }
    
    // Update lean capabilities based on cover width
    // For now, assume we can lean both ways
    CurrentCoverData.bCanLeanLeft = true;
    CurrentCoverData.bCanLeanRight = true;
    
    // Broadcast cover height changed
    OnCoverHeightChanged.Broadcast(CurrentCoverHeight);
}

bool UBlackRemasteredCoverComponent::TraceForCover(FVector& Start, FVector& End, FHitResult& HitResult) const
{
    FCollisionQueryParams TraceParams;
    
    if (OwnerCharacter)
    {
        TraceParams.AddIgnoredActor(OwnerCharacter);
    }
    
    // Trace for cover
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        TraceParams
    );
    
    return bHit;
}

// Timer callback
void UBlackRemasteredCoverComponent::OnCoverCheckTimer()
{
    UpdateCoverState();
}
