#include "BlackRemasteredMovementComponent.h"
#include "BlackRemasteredCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

UBlackRemasteredMovementComponent::UBlackRemasteredMovementComponent()
    : Super()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Initialize state
    OwnerCharacter = nullptr;
    CharacterMovement = nullptr;
    CurrentMovementState = MS_Walking;
    CurrentLeanDirection = LD_None;
    CurrentLeanAmount = 0.0f;
    CurrentVelocity = FVector::ZeroVector;
    CurrentSpeed = 0.0f;
    
    // Initialize movement settings
    MovementSettings.WalkSpeed = BlackRemasteredConstants::DEFAULT_WALK_SPEED;
    MovementSettings.RunSpeed = BlackRemasteredConstants::DEFAULT_RUN_SPEED;
    MovementSettings.SprintSpeed = BlackRemasteredConstants::DEFAULT_SPRINT_SPEED;
    MovementSettings.CrouchSpeed = BlackRemasteredConstants::DEFAULT_CROUCH_SPEED;
    MovementSettings.ProneSpeed = BlackRemasteredConstants::DEFAULT_PRONE_SPEED;
    MovementSettings.SlideSpeed = BlackRemasteredConstants::DEFAULT_SLIDE_SPEED;
    MovementSettings.MantleSpeed = BlackRemasteredConstants::DEFAULT_MANTLE_SPEED;
    MovementSettings.JumpZVelocity = BlackRemasteredConstants::DEFAULT_JUMP_VELOCITY;
    MovementSettings.GravityScale = BlackRemasteredConstants::DEFAULT_GRAVITY_SCALE;
    MovementSettings.SlideDuration = 1.0f;
    MovementSettings.SlideCooldown = 0.5f;
    MovementSettings.SlideStaminaCost = 20.0f;
    MovementSettings.MantleDuration = 0.5f;
    MovementSettings.MantleHeight = 100.0f;
    MovementSettings.MantleStaminaCost = 10.0f;
    MovementSettings.CoverSnapDistance = 50.0f;
    MovementSettings.CoverMoveSpeed = 100.0f;
    MovementSettings.LeanDistance = 20.0f;
    MovementSettings.LeanSpeed = 5.0f;
    MovementSettings.GroundFriction = 8.0f;
    MovementSettings.MaxAcceleration = 2048.0f;
    MovementSettings.BrakingDeceleration = 2048.0f;
    MovementSettings.BrakingFriction = 0.0f;
    MovementSettings.RotationalAcceleration = 32000.0f;
    MovementSettings.BrakingRotation = 32000.0f;
    
    // Initialize slide data
    SlideData.CurrentSlideTime = 0.0f;
    SlideData.SlideCooldownTime = 0.0f;
    SlideData.bCanSlide = true;
    SlideData.bIsSliding = false;
    
    // Initialize mantle data
    MantleData.CurrentMantleTime = 0.0f;
    MantleData.MantleState = MANTLE_None;
    MantleData.bIsMantling = false;
}

void UBlackRemasteredMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Get owner character
    if (AActor* Owner = GetOwner())
    {
        OwnerCharacter = Cast<ABlackRemasteredCharacter>(Owner);
    }
    
    // Get character movement component
    if (OwnerCharacter)
    {
        CharacterMovement = OwnerCharacter->GetCharacterMovement();
        
        // Apply movement settings
        if (CharacterMovement)
        {
            CharacterMovement->MaxWalkSpeed = MovementSettings.WalkSpeed;
            CharacterMovement->MaxWalkSpeedCrouched = MovementSettings.CrouchSpeed;
            CharacterMovement->JumpZVelocity = MovementSettings.JumpZVelocity;
            CharacterMovement->GravityScale = MovementSettings.GravityScale;
            CharacterMovement->GroundFriction = MovementSettings.GroundFriction;
            CharacterMovement->MaxAcceleration = MovementSettings.MaxAcceleration;
            CharacterMovement->BrakingDeceleration = MovementSettings.BrakingDeceleration;
            CharacterMovement->BrakingFriction = MovementSettings.BrakingFriction;
            CharacterMovement->RotationRate = FRotator(0.0f, MovementSettings.RotationalAcceleration, 0.0f);
            CharacterMovement->BrakingRotation = MovementSettings.BrakingRotation;
        }
    }
}

void UBlackRemasteredMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (!OwnerCharacter || !CharacterMovement)
    {
        return;
    }
    
    // Update movement
    UpdateMovement(DeltaTime);
    
    // Update rotation
    UpdateRotation(DeltaTime);
    
    // Update lean
    UpdateLean(DeltaTime);
    
    // Update slide
    if (SlideData.bIsSliding)
    {
        SlideData.CurrentSlideTime += DeltaTime;
        
        if (SlideData.CurrentSlideTime >= MovementSettings.SlideDuration)
        {
            StopSlide();
        }
    }
    
    // Update slide cooldown
    if (SlideData.SlideCooldownTime > 0)
    {
        SlideData.SlideCooldownTime -= DeltaTime;
        
        if (SlideData.SlideCooldownTime <= 0)
        {
            SlideData.bCanSlide = true;
        }
    }
    
    // Update mantle
    if (MantleData.bIsMantling)
    {
        MantleData.CurrentMantleTime += DeltaTime;
        
        if (MantleData.CurrentMantleTime >= MovementSettings.MantleDuration)
        {
            StopMantle();
        }
        else
        {
            ApplyMantleMovement(DeltaTime);
        }
    }
    
    // Update movement state
    UpdateMovementState();
}

void UBlackRemasteredMovementComponent::Initialize(ABlackRemasteredCharacter* Owner)
{
    OwnerCharacter = Owner;
    
    // Get character movement component
    if (OwnerCharacter)
    {
        CharacterMovement = OwnerCharacter->GetCharacterMovement();
        
        // Apply movement settings
        if (CharacterMovement)
        {
            CharacterMovement->MaxWalkSpeed = MovementSettings.WalkSpeed;
            CharacterMovement->MaxWalkSpeedCrouched = MovementSettings.CrouchSpeed;
            CharacterMovement->JumpZVelocity = MovementSettings.JumpZVelocity;
            CharacterMovement->GravityScale = MovementSettings.GravityScale;
            CharacterMovement->GroundFriction = MovementSettings.GroundFriction;
            CharacterMovement->MaxAcceleration = MovementSettings.MaxAcceleration;
            CharacterMovement->BrakingDeceleration = MovementSettings.BrakingDeceleration;
            CharacterMovement->BrakingFriction = MovementSettings.BrakingFriction;
            CharacterMovement->RotationRate = FRotator(0.0f, MovementSettings.RotationalAcceleration, 0.0f);
            CharacterMovement->BrakingRotation = MovementSettings.BrakingRotation;
        }
    }
}

void UBlackRemasteredMovementComponent::UpdateMovement(float DeltaTime)
{
    if (!OwnerCharacter || !CharacterMovement)
    {
        return;
    }
    
    // Update current velocity and speed
    CurrentVelocity = CharacterMovement->Velocity;
    CurrentSpeed = CurrentVelocity.Size();
    
    // Check if we're in a special movement state
    if (MantleData.bIsMantling)
    {
        // Mantling takes priority
        return;
    }
    
    if (SlideData.bIsSliding)
    {
        // Apply slide physics
        ApplySlidePhysics();
        return;
    }
    
    // Check if we're in cover
    if (IsInCover())
    {
        // Apply cover movement
        return;
    }
    
    // Update ground/air movement
    if (CharacterMovement->IsMovingOnGround())
    {
        UpdateGroundMovement();
    }
    else
    {
        UpdateAirMovement();
    }
}

void UBlackRemasteredMovementComponent::UpdateRotation(float DeltaTime)
{
    if (!OwnerCharacter || !CharacterMovement)
    {
        return;
    }
    
    // Apply lean offset
    ApplyLeanOffset(DeltaTime);
}

void UBlackRemasteredMovementComponent::UpdateMovementState()
{
    if (!OwnerCharacter || !CharacterMovement)
    {
        return;
    }
    
    EMovementState NewState = CurrentMovementState;
    
    // Check for special states first
    if (MantleData.bIsMantling)
    {
        NewState = MS_Mantling;
    }
    else if (SlideData.bIsSliding)
    {
        NewState = MS_Sliding;
    }
    else if (IsInCover())
    {
        NewState = MS_InCover;
    }
    else if (CurrentLeanDirection != LD_None)
    {
        NewState = MS_Leaning;
    }
    else if (!CharacterMovement->IsMovingOnGround())
    {
        if (CharacterMovement->Velocity.Z > 0)
        {
            NewState = MS_Jumping;
        }
        else
        {
            NewState = MS_Falling;
        }
    }
    else
    {
        // Ground movement
        if (CurrentSpeed > 0.1f)
        {
            if (OwnerCharacter->IsSprinting())
            {
                NewState = MS_Running;
            }
            else if (OwnerCharacter->IsCrouching())
            {
                NewState = MS_Crouching;
            }
            else if (OwnerCharacter->IsProne())
            {
                NewState = MS_Prone;
            }
            else
            {
                NewState = MS_Walking;
            }
        }
        else
        {
            if (OwnerCharacter->IsCrouching())
            {
                NewState = MS_Crouching;
            }
            else if (OwnerCharacter->IsProne())
            {
                NewState = MS_Prone;
            }
            else
            {
                NewState = MS_Walking;
            }
        }
    }
    
    // Check if state changed
    if (NewState != CurrentMovementState)
    {
        CurrentMovementState = NewState;
        OnMovementStateChanged.Broadcast();
    }
}

void UBlackRemasteredMovementComponent::UpdateSpeed()
{
    if (!CharacterMovement)
    {
        return;
    }
    
    // Update max speed based on state
    float MaxSpeed = GetMaxSpeed();
    CharacterMovement->MaxWalkSpeed = MaxSpeed;
}

void UBlackRemasteredMovementComponent::UpdateGroundMovement()
{
    if (!CharacterMovement)
    {
        return;
    }
    
    // Update speed based on current state
    UpdateSpeed();
}

void UBlackRemasteredMovementComponent::UpdateAirMovement()
{
    if (!CharacterMovement)
    {
        return;
    }
    
    // Check if we can mantle
    if (CanMantle() && OwnerCharacter)
    {
        // Auto-mantle if near ledge
        // This is handled by the character's Mantle() function
    }
}

void UBlackRemasteredMovementComponent::StartSlide()
{
    if (!CanSlide() || SlideData.bIsSliding)
    {
        return;
    }
    
    // Check stamina
    if (OwnerCharacter)
    {
        UBlackRemasteredHealthComponent* HealthComponent = OwnerCharacter->GetHealthComponent();
        if (HealthComponent)
        {
            // Check if we have enough stamina
            // This is handled in the character's StartSlide() function
        }
    }
    
    // Start slide
    SlideData.bIsSliding = true;
    SlideData.CurrentSlideTime = 0.0f;
    
    // Set state
    if (OwnerCharacter)
    {
        OwnerCharacter->SetCharacterState(CS_Sliding);
    }
    
    // Apply slide physics
    ApplySlidePhysics();
    
    // Start slide timer
    GetWorld()->GetTimerManager().SetTimer(
        SlideTimerHandle,
        this,
        &UBlackRemasteredMovementComponent::OnSlideTimer,
        MovementSettings.SlideDuration,
        false
    );
    
    // Start cooldown
    SlideData.bCanSlide = false;
    SlideData.SlideCooldownTime = MovementSettings.SlideCooldown;
    
    GetWorld()->GetTimerManager().SetTimer(
        SlideCooldownTimerHandle,
        this,
        &UBlackRemasteredMovementComponent::OnSlideCooldownTimer,
        MovementSettings.SlideCooldown,
        false
    );
    
    // Broadcast slide started
    OnSlideStarted.Broadcast(MovementSettings.SlideDuration);
}

void UBlackRemasteredMovementComponent::StopSlide()
{
    if (!SlideData.bIsSliding)
    {
        return;
    }
    
    // Stop slide
    SlideData.bIsSliding = false;
    SlideData.CurrentSlideTime = 0.0f;
    
    // Reset movement
    if (CharacterMovement)
    {
        CharacterMovement->SetMovementMode(MOVE_Walking);
        UpdateSpeed();
    }
    
    // Reset state
    if (OwnerCharacter)
    {
        OwnerCharacter->SetCharacterState(CS_Idle);
    }
    
    // Clear timers
    GetWorld()->GetTimerManager().ClearTimer(SlideTimerHandle);
    
    // Broadcast slide ended
    OnSlideEnded.Broadcast();
}

bool UBlackRemasteredMovementComponent::CanSlide() const
{
    if (!OwnerCharacter || !CharacterMovement)
    {
        return false;
    }
    
    if (SlideData.bIsSliding)
    {
        return false;
    }
    
    if (!SlideData.bCanSlide)
    {
        return false;
    }
    
    if (CharacterMovement->IsFalling())
    {
        return false;
    }
    
    if (OwnerCharacter->IsCrouching() || OwnerCharacter->IsProne())
    {
        return false;
    }
    
    if (CurrentSpeed < 100.0f)
    {
        return false;
    }
    
    // Check if we have enough stamina
    if (OwnerCharacter)
    {
        UBlackRemasteredHealthComponent* HealthComponent = OwnerCharacter->GetHealthComponent();
        if (HealthComponent)
        {
            // Check stamina
            // This is handled in the character
        }
    }
    
    return true;
}

void UBlackRemasteredMovementComponent::StartMantle()
{
    if (!CanMantle() || MantleData.bIsMantling)
    {
        return;
    }
    
    // Check stamina
    if (OwnerCharacter)
    {
        // Check if we have enough stamina
        // This is handled in the character's Mantle() function
    }
    
    // Find ledge location
    FVector LedgeLocation = FindLedgeLocation();
    
    if (LedgeLocation.IsNearlyZero())
    {
        return;
    }
    
    // Start mantle
    MantleData.bIsMantling = true;
    MantleData.CurrentMantleTime = 0.0f;
    MantleData.MantleState = MANTLE_Start;
    MantleData.MantleStartLocation = OwnerCharacter ? OwnerCharacter->GetActorLocation() : FVector::ZeroVector;
    MantleData.MantleTargetLocation = LedgeLocation;
    
    // Set state
    if (OwnerCharacter)
    {
        OwnerCharacter->SetCharacterState(CS_Mantling);
    }
    
    // Disable movement
    if (CharacterMovement)
    {
        CharacterMovement->SetMovementMode(MOVE_Custom, (uint8)EMovementMode::MOVE_Custom);
    }
    
    // Start mantle timer
    GetWorld()->GetTimerManager().SetTimer(
        MantleTimerHandle,
        this,
        &UBlackRemasteredMovementComponent::OnMantleTimer,
        MovementSettings.MantleDuration,
        false
    );
    
    // Broadcast mantle started
    OnMantleStarted.Broadcast(MovementSettings.MantleDuration);
}

void UBlackRemasteredMovementComponent::StopMantle()
{
    if (!MantleData.bIsMantling)
    {
        return;
    }
    
    // Stop mantle
    MantleData.bIsMantling = false;
    MantleData.CurrentMantleTime = 0.0f;
    MantleData.MantleState = MANTLE_None;
    
    // Reset movement
    if (CharacterMovement)
    {
        CharacterMovement->SetMovementMode(MOVE_Walking);
        UpdateSpeed();
    }
    
    // Set final position
    if (OwnerCharacter)
    {
        OwnerCharacter->SetActorLocation(MantleData.MantleTargetLocation);
        OwnerCharacter->SetCharacterState(CS_Idle);
    }
    
    // Clear timers
    GetWorld()->GetTimerManager().ClearTimer(MantleTimerHandle);
    
    // Broadcast mantle ended
    OnMantleEnded.Broadcast();
}

bool UBlackRemasteredMovementComponent::CanMantle() const
{
    if (!OwnerCharacter || !CharacterMovement)
    {
        return false;
    }
    
    if (MantleData.bIsMantling)
    {
        return false;
    }
    
    if (CharacterMovement->IsFalling())
    {
        return false;
    }
    
    if (OwnerCharacter->IsCrouching() || OwnerCharacter->IsProne() || OwnerCharacter->IsSliding())
    {
        return false;
    }
    
    // Check if there's a ledge to mantle
    return CheckForLedge();
}

void UBlackRemasteredMovementComponent::EnterCover()
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Find cover location
    FVector CoverLocation;
    FVector CoverNormal;
    
    if (FindCoverLocation(CoverLocation, CoverNormal))
    {
        // Move to cover
        if (OwnerCharacter)
        {
            OwnerCharacter->SetActorLocation(CoverLocation);
            OwnerCharacter->SetCharacterState(CS_InCover);
        }
    }
}

void UBlackRemasteredMovementComponent::ExitCover()
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Exit cover
    OwnerCharacter->SetCharacterState(CS_Idle);
}

bool UBlackRemasteredMovementComponent::IsInCover() const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    return OwnerCharacter->GetCharacterState() == CS_InCover;
}

void UBlackRemasteredMovementComponent::StartLean(ELeanDirection Direction)
{
    CurrentLeanDirection = Direction;
}

void UBlackRemasteredMovementComponent::StopLean()
{
    CurrentLeanDirection = LD_None;
}

void UBlackRemasteredMovementComponent::UpdateLean(float DeltaTime)
{
    if (CurrentLeanDirection == LD_None)
    {
        // Reset lean amount
        CurrentLeanAmount = FMath::Max(CurrentLeanAmount - MovementSettings.LeanSpeed * DeltaTime, 0.0f);
        
        if (CurrentLeanAmount <= 0.01f)
        {
            CurrentLeanAmount = 0.0f;
        }
    }
    else
    {
        // Apply lean
        float TargetLeanAmount = MovementSettings.LeanDistance;
        
        if (CurrentLeanDirection == LD_Left)
        {
            TargetLeanAmount = -MovementSettings.LeanDistance;
        }
        
        CurrentLeanAmount = FMath::Lerp(CurrentLeanAmount, TargetLeanAmount, MovementSettings.LeanSpeed * DeltaTime);
    }
    
    // Apply lean offset to character
    ApplyLeanOffset(DeltaTime);
}

float UBlackRemasteredMovementComponent::GetCurrentSpeed() const
{
    return CurrentSpeed;
}

float UBlackRemasteredMovementComponent::GetMaxSpeed() const
{
    if (!OwnerCharacter)
    {
        return MovementSettings.WalkSpeed;
    }
    
    if (OwnerCharacter->IsSprinting())
    {
        return MovementSettings.SprintSpeed;
    }
    
    if (OwnerCharacter->IsCrouching())
    {
        return MovementSettings.CrouchSpeed;
    }
    
    if (OwnerCharacter->IsProne())
    {
        return MovementSettings.ProneSpeed;
    }
    
    if (SlideData.bIsSliding)
    {
        return MovementSettings.SlideSpeed;
    }
    
    if (MantleData.bIsMantling)
    {
        return MovementSettings.MantleSpeed;
    }
    
    return MovementSettings.WalkSpeed;
}

bool UBlackRemasteredMovementComponent::IsMoving() const
{
    return CurrentSpeed > 0.1f;
}

bool UBlackRemasteredMovementComponent::IsSprinting() const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    return OwnerCharacter->IsSprinting();
}

bool UBlackRemasteredMovementComponent::IsCrouching() const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    return OwnerCharacter->IsCrouching();
}

bool UBlackRemasteredMovementComponent::IsProne() const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    return OwnerCharacter->IsProne();
}

bool UBlackRemasteredMovementComponent::IsJumping() const
{
    if (!CharacterMovement)
    {
        return false;
    }
    
    return CharacterMovement->IsJumpProvidingForce();
}

bool UBlackRemasteredMovementComponent::IsFalling() const
{
    if (!CharacterMovement)
    {
        return false;
    }
    
    return CharacterMovement->IsFalling();
}

// Timer callbacks
void UBlackRemasteredMovementComponent::OnSlideTimer()
{
    StopSlide();
}

void UBlackRemasteredMovementComponent::OnSlideCooldownTimer()
{
    SlideData.bCanSlide = true;
}

void UBlackRemasteredMovementComponent::OnMantleTimer()
{
    // Mantle complete
    MantleData.MantleState = MANTLE_End;
}

// Helper functions
void UBlackRemasteredMovementComponent::ApplySlidePhysics()
{
    if (!CharacterMovement || !OwnerCharacter)
    {
        return;
    }
    
    // Set movement mode to custom for slide
    CharacterMovement->SetMovementMode(MOVE_Custom, (uint8)EMovementMode::MOVE_Custom);
    
    // Apply slide velocity
    FVector SlideDirection = OwnerCharacter->GetActorForwardVector();
    SlideDirection.Z = 0.0f;
    SlideDirection.Normalize();
    
    CharacterMovement->Velocity = SlideDirection * MovementSettings.SlideSpeed;
    
    // Reduce friction for sliding
    CharacterMovement->GroundFriction = 0.1f;
}

void UBlackRemasteredMovementComponent::ApplyMantleMovement(float DeltaTime)
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Calculate progress
    float Progress = MantleData.CurrentMantleTime / MovementSettings.MantleDuration;
    
    // Calculate current location
    FVector CurrentLocation = FMath::Lerp(
        MantleData.MantleStartLocation,
        MantleData.MantleTargetLocation,
        Progress
    );
    
    // Add vertical offset based on mantle state
    float VerticalOffset = 0.0f;
    
    switch (MantleData.MantleState)
    {
        case MANTLE_Start:
            // Start mantle - moving up
            VerticalOffset = FMath::Sin(Progress * PI) * MovementSettings.MantleHeight;
            break;
            
        case MANTLE_Climb:
            // Climbing - at peak
            VerticalOffset = MovementSettings.MantleHeight;
            break;
            
        case MANTLE_End:
            // End mantle - moving down
            VerticalOffset = FMath::Sin((1.0f - Progress) * PI) * MovementSettings.MantleHeight;
            break;
            
        default:
            break;
    }
    
    // Apply vertical offset
    CurrentLocation.Z += VerticalOffset;
    
    // Set character location
    OwnerCharacter->SetActorLocation(CurrentLocation);
    
    // Update mantle state based on progress
    if (Progress >= 0.5f && MantleData.MantleState == MANTLE_Start)
    {
        MantleData.MantleState = MANTLE_Climb;
    }
    else if (Progress >= 0.8f && MantleData.MantleState == MANTLE_Climb)
    {
        MantleData.MantleState = MANTLE_End;
    }
}

void UBlackRemasteredMovementComponent::ApplyLeanOffset(float DeltaTime)
{
    if (!OwnerCharacter || !OwnerCharacter->GetFollowCamera())
    {
        return;
    }
    
    // Calculate lean offset
    FVector LeanOffset = FVector::ZeroVector;
    
    switch (CurrentLeanDirection)
    {
        case LD_Left:
            LeanOffset = FVector(-CurrentLeanAmount, 0.0f, 0.0f);
            break;
            
        case LD_Right:
            LeanOffset = FVector(CurrentLeanAmount, 0.0f, 0.0f);
            break;
            
        default:
            break;
    }
    
    // Apply lean offset to camera
    FVector CurrentOffset = OwnerCharacter->GetFollowCamera()->GetRelativeLocation();
    CurrentOffset += LeanOffset;
    OwnerCharacter->GetFollowCamera()->SetRelativeLocation(CurrentOffset);
}

bool UBlackRemasteredMovementComponent::CheckForLedge() const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    // Check for ledge in front of character
    FVector StartLocation = OwnerCharacter->GetActorLocation();
    StartLocation.Z += 50.0f; // Raise to chest height
    
    FVector EndLocation = StartLocation + (OwnerCharacter->GetActorForwardVector() * 100.0f);
    EndLocation.Z += MovementSettings.MantleHeight;
    
    // Trace for ledge
    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(OwnerCharacter);
    
    // Trace down from end location
    FVector TraceEnd = EndLocation;
    TraceEnd.Z -= MovementSettings.MantleHeight * 2.0f;
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        EndLocation,
        TraceEnd,
        ECC_Visibility,
        TraceParams
    );
    
    // If we hit something, check if it's a valid ledge
    if (bHit)
    {
        // Check if the surface is horizontal enough
        float SurfaceAngle = FMath::Abs(FMath::Asin(HitResult.Normal.Z));
        
        if (SurfaceAngle < 30.0f) // Less than 30 degrees
        {
            // Valid ledge found
            return true;
        }
    }
    
    return false;
}

FVector UBlackRemasteredMovementComponent::FindLedgeLocation() const
{
    if (!OwnerCharacter)
    {
        return FVector::ZeroVector;
    }
    
    // Check for ledge in front of character
    FVector StartLocation = OwnerCharacter->GetActorLocation();
    StartLocation.Z += 50.0f; // Raise to chest height
    
    FVector EndLocation = StartLocation + (OwnerCharacter->GetActorForwardVector() * 100.0f);
    EndLocation.Z += MovementSettings.MantleHeight;
    
    // Trace for ledge
    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(OwnerCharacter);
    
    // Trace down from end location
    FVector TraceEnd = EndLocation;
    TraceEnd.Z -= MovementSettings.MantleHeight * 2.0f;
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        EndLocation,
        TraceEnd,
        ECC_Visibility,
        TraceParams
    );
    
    // If we hit something, return the location above the surface
    if (bHit)
    {
        // Check if the surface is horizontal enough
        float SurfaceAngle = FMath::Abs(FMath::Asin(HitResult.Normal.Z));
        
        if (SurfaceAngle < 30.0f) // Less than 30 degrees
        {
            // Calculate ledge location
            FVector LedgeLocation = HitResult.ImpactPoint;
            LedgeLocation.Z += 100.0f; // Add character height
            
            // Move forward slightly
            LedgeLocation += OwnerCharacter->GetActorForwardVector() * 50.0f;
            
            return LedgeLocation;
        }
    }
    
    return FVector::ZeroVector;
}

bool UBlackRemasteredMovementComponent::FindCoverLocation(FVector& CoverLocation, FVector& CoverNormal) const
{
    if (!OwnerCharacter)
    {
        return false;
    }
    
    // Check for cover in front of character
    FVector StartLocation = OwnerCharacter->GetActorLocation();
    FVector TraceDirection = OwnerCharacter->GetActorForwardVector();
    
    // Trace for cover
    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(OwnerCharacter);
    
    float TraceDistance = 200.0f;
    FVector EndLocation = StartLocation + (TraceDirection * TraceDistance);
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        TraceParams
    );
    
    if (bHit)
    {
        // Check if the surface is vertical enough for cover
        float SurfaceAngle = FMath::Abs(FMath::Asin(HitResult.Normal.Z));
        
        if (SurfaceAngle > 60.0f) // More than 60 degrees (mostly vertical)
        {
            // Valid cover found
            CoverLocation = HitResult.ImpactPoint;
            CoverNormal = HitResult.Normal;
            return true;
        }
    }
    
    return false;
}

void UBlackRemasteredMovementComponent::DetectCover()
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Check for cover in front of character
    FVector CoverLocation;
    FVector CoverNormal;
    
    if (FindCoverLocation(CoverLocation, CoverNormal))
    {
        // Cover is available
        // This is handled by the character's CheckForCover() function
    }
}
