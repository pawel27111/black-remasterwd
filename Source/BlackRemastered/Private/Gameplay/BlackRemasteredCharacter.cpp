#include "BlackRemasteredCharacter.h"
#include "Gameplay/BlackRemasteredWeaponComponent.h"
#include "Gameplay/BlackRemasteredHealthComponent.h"
#include "Gameplay/BlackRemasteredMovementComponent.h"
#include "Gameplay/BlackRemasteredInventoryComponent.h"
#include "Gameplay/BlackRemasteredInteractionComponent.h"
#include "Gameplay/BlackRemasteredCoverComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Core/BlackRemasteredGameMode.h"
#include "Core/BlackRemasteredGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundCue.h"

ABlackRemasteredCharacter::ABlackRemasteredCharacter()
    : Super()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
    
    // Initialize state
    CharacterState = CS_Idle;
    LeanDirection = LD_None;
    bIsAiming = false;
    bIsSprinting = false;
    bIsCrouching = false;
    bIsProne = false;
    bIsSliding = false;
    bIsInCover = false;
    
    MoveForwardValue = 0.0f;
    MoveRightValue = 0.0f;
    LookUpValue = 0.0f;
    LookRightValue = 0.0f;
    
    bSprintInput = false;
    bCrouchInput = false;
    bProneInput = false;
    bAimInput = false;
    bLeanLeftInput = false;
    bLeanRightInput = false;
    
    // Movement settings
    WalkSpeed = BlackRemasteredConstants::DEFAULT_WALK_SPEED;
    RunSpeed = BlackRemasteredConstants::DEFAULT_RUN_SPEED;
    CrouchSpeed = BlackRemasteredConstants::DEFAULT_CROUCH_SPEED;
    ProneSpeed = BlackRemasteredConstants::DEFAULT_PRONE_SPEED;
    SprintSpeed = BlackRemasteredConstants::DEFAULT_SPRINT_SPEED;
    SlideSpeed = BlackRemasteredConstants::DEFAULT_SLIDE_SPEED;
    MantleSpeed = BlackRemasteredConstants::DEFAULT_MANTLE_SPEED;
    
    // Camera settings
    CameraFOV = BlackRemasteredConstants::DEFAULT_CAMERA_FOV;
    AimFOV = BlackRemasteredConstants::DEFAULT_AIM_FOV;
    CameraLagSpeed = BlackRemasteredConstants::DEFAULT_CAMERA_LAG_SPEED;
    CameraLagMaxDistance = BlackRemasteredConstants::DEFAULT_CAMERA_LAG_MAX_DISTANCE;
    CameraOffset = FVector(0.0f, 0.0f, 60.0f);
    AimCameraOffset = FVector(0.0f, 0.0f, 50.0f);
    
    // Camera sway settings
    CameraSwayAmount = 5.0f;
    CameraSwaySpeed = 2.0f;
    CameraBobAmount = 2.0f;
    CameraBobSpeed = 3.0f;
    
    // Slide settings
    SlideDuration = 1.0f;
    SlideCooldown = 0.5f;
    
    // Stamina settings
    MaxStamina = BlackRemasteredConstants::DEFAULT_MAX_STAMINA;
    CurrentStamina = MaxStamina;
    StaminaRegenRate = BlackRemasteredConstants::DEFAULT_STAMINA_REGEN_RATE;
    StaminaRegenDelay = BlackRemasteredConstants::DEFAULT_STAMINA_REGEN_DELAY;
    SprintStaminaCost = BlackRemasteredConstants::DEFAULT_SPRINT_STAMINA_COST;
    SlideStaminaCost = BlackRemasteredConstants::DEFAULT_SLIDE_STAMINA_COST;
    
    // Footstep settings
    LastFootstepTime = 0.0f;
    FootstepInterval = 0.4f;
    
    // Create camera boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 80.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bInheritPitch = true;
    CameraBoom->bInheritRoll = true;
    CameraBoom->bInheritYaw = true;
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->CameraLagMaxDistance = CameraLagMaxDistance;
    CameraBoom->CameraLagSpeed = CameraLagSpeed;
    
    // Create follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->FieldOfView = CameraFOV;
    
    // Create components
    WeaponComponent = CreateDefaultSubobject<UBlackRemasteredWeaponComponent>(TEXT("WeaponComponent"));
    HealthComponent = CreateDefaultSubobject<UBlackRemasteredHealthComponent>(TEXT("HealthComponent"));
    MovementComponent = CreateDefaultSubobject<UBlackRemasteredMovementComponent>(TEXT("MovementComponent"));
    InventoryComponent = CreateDefaultSubobject<UBlackRemasteredInventoryComponent>(TEXT("InventoryComponent"));
    InteractionComponent = CreateDefaultSubobject<UBlackRemasteredInteractionComponent>(TEXT("InteractionComponent"));
    CoverComponent = CreateDefaultSubobject<UBlackRemasteredCoverComponent>(TEXT("CoverComponent"));
    
    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = BlackRemasteredConstants::DEFAULT_JUMP_VELOCITY;
    GetCharacterMovement()->GravityScale = BlackRemasteredConstants::DEFAULT_GRAVITY_SCALE;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
    GetCharacterMovement()->bCanWalkOffLedges = true;
    GetCharacterMovement()->bCanCrouch = true;
    GetCharacterMovement()->bCanJump = true;
    GetCharacterMovement()->bCanSprint = true;
}

void ABlackRemasteredCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize components
    if (WeaponComponent)
    {
        WeaponComponent->Initialize(this);
    }
    
    if (HealthComponent)
    {
        HealthComponent->Initialize(this);
    }
    
    if (MovementComponent)
    {
        MovementComponent->Initialize(this);
    }
    
    if (InventoryComponent)
    {
        InventoryComponent->Initialize(this);
    }
    
    if (InteractionComponent)
    {
        InteractionComponent->Initialize(this);
    }
    
    if (CoverComponent)
    {
        CoverComponent->Initialize(this);
    }
    
    // Set initial camera offset
    CameraBoom->SetRelativeLocation(CameraOffset);
    
    // Start with default weapon
    if (InventoryComponent)
    {
        InventoryComponent->EquipDefaultWeapon();
    }
}

void ABlackRemasteredCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (Controller && Controller->IsLocalPlayerController())
    {
        // Update movement
        UpdateMovement(DeltaTime);
        
        // Update stamina
        UpdateStamina(DeltaTime);
        
        // Update state
        UpdateState(DeltaTime);
        
        // Update camera
        UpdateCamera(DeltaTime);
        
        // Apply camera effects
        ApplyCameraEffects(DeltaTime);
        
        // Update lean
        UpdateLean(DeltaTime);
        
        // Update animation
        UpdateAnimation();
        
        // Check for cover
        CheckForCover();
    }
}

void ABlackRemasteredCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    InputComponent = PlayerInputComponent;
    
    if (!InputComponent)
    {
        return;
    }
    
    // Movement bindings
    InputComponent->BindAxis("MoveForward", this, &ABlackRemasteredCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &ABlackRemasteredCharacter::MoveRight);
    InputComponent->BindAxis("LookUp", this, &ABlackRemasteredCharacter::LookUp);
    InputComponent->BindAxis("LookRight", this, &ABlackRemasteredCharacter::LookRight);
    
    // Action bindings
    InputComponent->BindAction("Sprint", IE_Pressed, this, &ABlackRemasteredCharacter::StartSprint);
    InputComponent->BindAction("Sprint", IE_Released, this, &ABlackRemasteredCharacter::StopSprint);
    InputComponent->BindAction("Crouch", IE_Pressed, this, &ABlackRemasteredCharacter::StartCrouch);
    InputComponent->BindAction("Crouch", IE_Released, this, &ABlackRemasteredCharacter::StopCrouch);
    InputComponent->BindAction("Prone", IE_Pressed, this, &ABlackRemasteredCharacter::StartProne);
    InputComponent->BindAction("Prone", IE_Released, this, &ABlackRemasteredCharacter::StopProne);
    InputComponent->BindAction("Slide", IE_Pressed, this, &ABlackRemasteredCharacter::StartSlide);
    InputComponent->BindAction("Aim", IE_Pressed, this, &ABlackRemasteredCharacter::StartAim);
    InputComponent->BindAction("Aim", IE_Released, this, &ABlackRemasteredCharacter::StopAim);
    InputComponent->BindAction("LeanLeft", IE_Pressed, this, &ABlackRemasteredCharacter::StartLeanLeft);
    InputComponent->BindAction("LeanLeft", IE_Released, this, &ABlackRemasteredCharacter::StopLeanLeft);
    InputComponent->BindAction("LeanRight", IE_Pressed, this, &ABlackRemasteredCharacter::StartLeanRight);
    InputComponent->BindAction("LeanRight", IE_Released, this, &ABlackRemasteredCharacter::StopLeanRight);
    InputComponent->BindAction("Jump", IE_Pressed, this, &ABlackRemasteredCharacter::Jump);
    InputComponent->BindAction("Mantle", IE_Pressed, this, &ABlackRemasteredCharacter::Mantle);
    InputComponent->BindAction("Interact", IE_Pressed, this, &ABlackRemasteredCharacter::Interact);
    InputComponent->BindAction("Reload", IE_Pressed, this, &ABlackRemasteredCharacter::Reload);
    InputComponent->BindAction("Fire", IE_Pressed, this, &ABlackRemasteredCharacter::Fire);
    InputComponent->BindAction("Fire", IE_Released, this, &ABlackRemasteredCharacter::Fire);
    InputComponent->BindAction("Aim", IE_Pressed, this, &ABlackRemasteredCharacter::Aim);
    InputComponent->BindAction("Melee", IE_Pressed, this, &ABlackRemasteredCharacter::Melee);
    InputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ABlackRemasteredCharacter::ThrowGrenade);
    InputComponent->BindAction("NextWeapon", IE_Pressed, this, &ABlackRemasteredCharacter::NextWeapon);
    InputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &ABlackRemasteredCharacter::PreviousWeapon);
    InputComponent->BindAction("ToggleFireMode", IE_Pressed, this, &ABlackRemasteredCharacter::ToggleFireMode);
    
    // Weapon selection bindings
    InputComponent->BindAction("Weapon1", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 0);
    InputComponent->BindAction("Weapon2", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 1);
    InputComponent->BindAction("Weapon3", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 2);
    InputComponent->BindAction("Weapon4", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 3);
    InputComponent->BindAction("Weapon5", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 4);
    InputComponent->BindAction("Weapon6", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 5);
    InputComponent->BindAction("Weapon7", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 6);
    InputComponent->BindAction("Weapon8", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 7);
    InputComponent->BindAction("Weapon9", IE_Pressed, this, &ABlackRemasteredCharacter::SelectWeapon, 8);
}

void ABlackRemasteredCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    
    // Initialize player-specific settings
    if (ABlackRemasteredPlayerController* BRController = Cast<ABlackRemasteredPlayerController>(NewController))
    {
        BRController->SetPawn(this);
    }
}

void ABlackRemasteredCharacter::UnPossessed()
{
    // Cleanup
    if (WeaponComponent)
    {
        WeaponComponent->Cleanup();
    }
    
    Super::UnPossessed();
}

void ABlackRemasteredCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    
    // Play landing sound based on fall height
    if (Hit.IsValidBlockingHit())
    {
        float FallHeight = GetCharacterMovement()->GetLastFallHeight();
        if (FallHeight > 200.0f)
        {
            // Heavy landing
            // TODO: Play heavy landing sound
        }
        else if (FallHeight > 100.0f)
        {
            // Medium landing
            // TODO: Play medium landing sound
        }
        else
        {
            // Light landing
            // TODO: Play light landing sound
        }
    }
}

// Input handlers
void ABlackRemasteredCharacter::MoveForward(float Value)
{
    MoveForwardValue = Value;
}

void ABlackRemasteredCharacter::MoveRight(float Value)
{
    MoveRightValue = Value;
}

void ABlackRemasteredCharacter::LookUp(float Value)
{
    LookUpValue = Value;
    
    if (Controller)
    {
        AddControllerPitchInput(Value);
    }
}

void ABlackRemasteredCharacter::LookRight(float Value)
{
    LookRightValue = Value;
    
    if (Controller)
    {
        AddControllerYawInput(Value);
    }
}

void ABlackRemasteredCharacter::StartSprint()
{
    bSprintInput = true;
}

void ABlackRemasteredCharacter::StopSprint()
{
    bSprintInput = false;
}

void ABlackRemasteredCharacter::StartCrouch()
{
    bCrouchInput = true;
}

void ABlackRemasteredCharacter::StopCrouch()
{
    bCrouchInput = false;
}

void ABlackRemasteredCharacter::StartProne()
{
    bProneInput = true;
}

void ABlackRemasteredCharacter::StopProne()
{
    bProneInput = false;
}

void ABlackRemasteredCharacter::StartSlide()
{
    if (CanSlide() && CurrentStamina >= SlideStaminaCost)
    {
        CurrentStamina -= SlideStaminaCost;
        
        // Trigger slide
        if (MovementComponent)
        {
            MovementComponent->StartSlide();
        }
        
        // Set state
        SetCharacterState(CS_Sliding);
        
        // Start slide timer
        GetWorld()->GetTimerManager().SetTimer(
            SlideTimerHandle,
            this,
            &ABlackRemasteredCharacter::OnSlideTimer,
            SlideDuration,
            false
        );
        
        // Start cooldown timer
        GetWorld()->GetTimerManager().SetTimer(
            SlideCooldownTimerHandle,
            this,
            &ABlackRemasteredCharacter::OnSlideCooldownTimer,
            SlideCooldown,
            false
        );
    }
}

void ABlackRemasteredCharacter::StartAim()
{
    bAimInput = true;
    bIsAiming = true;
    
    // Apply FOV change
    if (FollowCamera)
    {
        FollowCamera->SetFieldOfView(AimFOV);
    }
    
    // Update camera offset
    CameraBoom->SetRelativeLocation(AimCameraOffset);
}

void ABlackRemasteredCharacter::StopAim()
{
    bAimInput = false;
    bIsAiming = false;
    
    // Restore FOV
    if (FollowCamera)
    {
        FollowCamera->SetFieldOfView(CameraFOV);
    }
    
    // Restore camera offset
    CameraBoom->SetRelativeLocation(CameraOffset);
}

void ABlackRemasteredCharacter::StartLeanLeft()
{
    bLeanLeftInput = true;
    SetLeanDirection(LD_Left);
}

void ABlackRemasteredCharacter::StopLeanLeft()
{
    bLeanLeftInput = false;
    if (!bLeanRightInput)
    {
        SetLeanDirection(LD_None);
    }
}

void ABlackRemasteredCharacter::StartLeanRight()
{
    bLeanRightInput = true;
    SetLeanDirection(LD_Right);
}

void ABlackRemasteredCharacter::StopLeanRight()
{
    bLeanRightInput = false;
    if (!bLeanLeftInput)
    {
        SetLeanDirection(LD_None);
    }
}

void ABlackRemasteredCharacter::Jump()
{
    if (CanJump())
    {
        Super::Jump();
    }
}

void ABlackRemasteredCharacter::Mantle()
{
    if (CanMantle())
    {
        if (MovementComponent)
        {
            MovementComponent->StartMantle();
        }
        
        SetCharacterState(CS_Mantling);
        
        GetWorld()->GetTimerManager().SetTimer(
            MantleTimerHandle,
            this,
            &ABlackRemasteredCharacter::OnMantleTimer,
            0.5f,
            false
        );
    }
}

void ABlackRemasteredCharacter::Interact()
{
    if (InteractionComponent)
    {
        InteractionComponent->Interact();
    }
}

void ABlackRemasteredCharacter::Reload()
{
    if (WeaponComponent)
    {
        WeaponComponent->Reload();
    }
}

void ABlackRemasteredCharacter::Fire()
{
    if (WeaponComponent)
    {
        WeaponComponent->Fire();
    }
}

void ABlackRemasteredCharacter::Aim()
{
    // Toggle aim
    if (bIsAiming)
    {
        StopAim();
    }
    else
    {
        StartAim();
    }
}

void ABlackRemasteredCharacter::Melee()
{
    if (WeaponComponent)
    {
        WeaponComponent->Melee();
    }
}

void ABlackRemasteredCharacter::ThrowGrenade()
{
    if (InventoryComponent)
    {
        InventoryComponent->ThrowGrenade();
    }
}

void ABlackRemasteredCharacter::NextWeapon()
{
    if (InventoryComponent)
    {
        InventoryComponent->NextWeapon();
    }
}

void ABlackRemasteredCharacter::PreviousWeapon()
{
    if (InventoryComponent)
    {
        InventoryComponent->PreviousWeapon();
    }
}

void ABlackRemasteredCharacter::SelectWeapon(int32 Index)
{
    if (InventoryComponent)
    {
        InventoryComponent->SelectWeapon(Index);
    }
}

void ABlackRemasteredCharacter::ToggleFireMode()
{
    if (WeaponComponent)
    {
        WeaponComponent->ToggleFireMode();
    }
}

// State management
void ABlackRemasteredCharacter::SetCharacterState(ECharacterState NewState)
{
    ECharacterState OldState = CharacterState;
    CharacterState = NewState;
    
    // Handle state transitions
    switch (NewState)
    {
        case CS_Idle:
            bIsSprinting = false;
            bIsSliding = false;
            break;
            
        case CS_Walking:
            bIsSprinting = false;
            break;
            
        case CS_Running:
            bIsSprinting = true;
            break;
            
        case CS_Crouching:
            bIsCrouching = true;
            bIsSprinting = false;
            break;
            
        case CS_Prone:
            bIsProne = true;
            bIsCrouching = false;
            bIsSprinting = false;
            break;
            
        case CS_Sliding:
            bIsSliding = true;
            bIsSprinting = false;
            break;
            
        case CS_Mantling:
            bIsSprinting = false;
            break;
            
        case CS_InCover:
            bIsSprinting = false;
            bIsInCover = true;
            break;
            
        case CS_Leaning:
            break;
            
        case CS_Interacting:
            bIsSprinting = false;
            break;
            
        case CS_Dead:
            bIsSprinting = false;
            bIsCrouching = false;
            bIsProne = false;
            bIsAiming = false;
            bIsSliding = false;
            bIsInCover = false;
            break;
            
        default:
            break;
    }
    
    // Broadcast state change
    OnCharacterStateChanged.Broadcast(OldState, NewState);
}

void ABlackRemasteredCharacter::SetLeanDirection(ELeanDirection NewDirection)
{
    ELeanDirection OldDirection = LeanDirection;
    LeanDirection = NewDirection;
    
    // Broadcast lean change
    OnLeanDirectionChanged.Broadcast(OldDirection, NewDirection);
}

// Update functions
void ABlackRemasteredCharacter::UpdateMovement(float DeltaTime)
{
    if (!Controller || !GetCharacterMovement())
    {
        return;
    }
    
    // Get movement component
    UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
    
    // Check if we can move
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting || CharacterState == CS_Mantling)
    {
        return;
    }
    
    // Handle sprint
    if (bSprintInput && CanSprint() && CurrentStamina > 0)
    {
        if (!bIsSprinting)
        {
            SetCharacterState(CS_Running);
        }
        
        CharacterMovement->MaxWalkSpeed = SprintSpeed;
    }
    else
    {
        if (bIsSprinting)
        {
            SetCharacterState(CS_Walking);
        }
        
        // Determine base speed based on state
        if (bIsProne)
        {
            CharacterMovement->MaxWalkSpeed = ProneSpeed;
        }
        else if (bIsCrouching)
        {
            CharacterMovement->MaxWalkSpeed = CrouchSpeed;
        }
        else if (bIsSliding)
        {
            CharacterMovement->MaxWalkSpeed = SlideSpeed;
        }
        else if (bIsInCover)
        {
            CharacterMovement->MaxWalkSpeed = CrouchSpeed;
        }
        else
        {
            CharacterMovement->MaxWalkSpeed = WalkSpeed;
        }
    }
    
    // Handle crouch
    if (bCrouchInput && CanCrouch())
    {
        if (!bIsCrouching)
        {
            Crouch();
            SetCharacterState(CS_Crouching);
        }
    }
    else if (bIsCrouching && !bCrouchInput)
    {
        UnCrouch();
        SetCharacterState(CS_Idle);
    }
    
    // Handle prone
    if (bProneInput && CanProne())
    {
        if (!bIsProne)
        {
            // Go prone
            SetCharacterState(CS_Prone);
            
            // Force crouch to enable prone
            if (!bIsCrouching)
            {
                Crouch();
            }
        }
    }
    else if (bIsProne && !bProneInput)
    {
        // Get up from prone
        SetCharacterState(CS_Crouching);
    }
    
    // Apply movement
    if (MoveForwardValue != 0.0f || MoveRightValue != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        
        const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation) * MoveForwardValue + 
                                   UKismetMathLibrary::GetRightVector(YawRotation) * MoveRightValue;
        
        AddMovementInput(Direction.GetSafeNormal(), FMath::Clamp(Direction.Size(), 0.0f, 1.0f));
        
        // Play footstep sounds
        if (GetWorld()->GetTimeSeconds() - LastFootstepTime >= FootstepInterval)
        {
            PlayFootstepSound();
            LastFootstepTime = GetWorld()->GetTimeSeconds();
        }
    }
    
    // Update state based on movement
    if (GetVelocity().Size() > 0.1f)
    {
        if (bIsSprinting)
        {
            SetCharacterState(CS_Running);
        }
        else if (bIsCrouching)
        {
            SetCharacterState(CS_Crouching);
        }
        else if (bIsProne)
        {
            SetCharacterState(CS_Prone);
        }
        else if (bIsSliding)
        {
            SetCharacterState(CS_Sliding);
        }
        else if (bIsInCover)
        {
            SetCharacterState(CS_InCover);
        }
        else
        {
            SetCharacterState(CS_Walking);
        }
    }
    else
    {
        if (bIsCrouching)
        {
            SetCharacterState(CS_Crouching);
        }
        else if (bIsProne)
        {
            SetCharacterState(CS_Prone);
        }
        else if (bIsInCover)
        {
            SetCharacterState(CS_InCover);
        }
        else
        {
            SetCharacterState(CS_Idle);
        }
    }
}

void ABlackRemasteredCharacter::UpdateStamina(float DeltaTime)
{
    if (CurrentStamina < MaxStamina)
    {
        // Check if we can regenerate stamina
        if (!bIsSprinting && !bSprintInput)
        {
            CurrentStamina = FMath::Min(CurrentStamina + StaminaRegenRate * DeltaTime, MaxStamina);
        }
    }
    
    // Consume stamina while sprinting
    if (bIsSprinting && CurrentStamina > 0)
    {
        CurrentStamina = FMath::Max(CurrentStamina - SprintStaminaCost * DeltaTime, 0.0f);
        
        if (CurrentStamina <= 0)
        {
            // Force stop sprinting
            bIsSprinting = false;
            SetCharacterState(CS_Walking);
        }
    }
}

void ABlackRemasteredCharacter::UpdateState(float DeltaTime)
{
    // Update character state based on various conditions
    
    // Check for cover
    if (CoverComponent)
    {
        CoverComponent->UpdateCoverState();
    }
}

void ABlackRemasteredCharacter::UpdateCamera(float DeltaTime)
{
    if (!FollowCamera || !CameraBoom)
    {
        return;
    }
    
    // Update camera position based on state
    if (bIsAiming)
    {
        // Aiming camera offset
        CameraBoom->SetRelativeLocation(FMath::Lerp(CameraBoom->GetRelativeLocation(), AimCameraOffset, DeltaTime * 10.0f));
    }
    else
    {
        // Normal camera offset
        CameraBoom->SetRelativeLocation(FMath::Lerp(CameraBoom->GetRelativeLocation(), CameraOffset, DeltaTime * 10.0f));
    }
}

void ABlackRemasteredCharacter::ApplyCameraEffects(float DeltaTime)
{
    if (!FollowCamera)
    {
        return;
    }
    
    // Apply camera sway
    ApplyCameraSway(DeltaTime);
    
    // Apply camera bob
    ApplyCameraBob(DeltaTime);
}

void ABlackRemasteredCharacter::UpdateLean(float DeltaTime)
{
    // Handle lean transitions
    switch (LeanDirection)
    {
        case LD_Left:
            // Apply left lean offset
            break;
            
        case LD_Right:
            // Apply right lean offset
            break;
            
        default:
            // Reset to center
            break;
    }
}

void ABlackRemasteredCharacter::UpdateAnimation()
{
    // Update animation state based on character state
    // This would typically be handled by an Animation Blueprint
}

// Camera effect helpers
void ABlackRemasteredCharacter::ApplyCameraSway(float DeltaTime)
{
    if (!FollowCamera || CharacterState == CS_Dead)
    {
        return;
    }
    
    // Calculate sway based on movement
    float SwayX = 0.0f;
    float SwayY = 0.0f;
    
    if (GetVelocity().Size() > 0)
    {
        // Sway increases with speed
        float SpeedFactor = GetVelocity().Size() / SprintSpeed;
        SwayX = FMath::Sin(GetWorld()->GetTimeSeconds() * CameraSwaySpeed) * CameraSwayAmount * SpeedFactor;
        SwayY = FMath::Cos(GetWorld()->GetTimeSeconds() * CameraSwaySpeed * 0.7f) * CameraSwayAmount * SpeedFactor * 0.5f;
    }
    
    // Apply sway to camera
    FollowCamera->SetRelativeLocation(FVector(SwayX, SwayY, 0.0f));
}

void ABlackRemasteredCharacter::ApplyCameraBob(float DeltaTime)
{
    if (!FollowCamera || CharacterState == CS_Dead)
    {
        return;
    }
    
    // Calculate bob based on movement
    if (GetVelocity().Size() > 0)
    {
        float BobAmount = CameraBobAmount * (GetVelocity().Size() / SprintSpeed);
        float BobOffset = FMath::Sin(GetWorld()->GetTimeSeconds() * CameraBobSpeed) * BobAmount;
        
        // Apply bob to camera
        FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BobOffset));
    }
}

// Movement helpers
bool ABlackRemasteredCharacter::CanSprint() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting || CharacterState == CS_Mantling)
    {
        return false;
    }
    
    if (bIsCrouching || bIsProne || bIsSliding)
    {
        return false;
    }
    
    if (CurrentStamina <= 0)
    {
        return false;
    }
    
    if (GetCharacterMovement()->IsFalling())
    {
        return false;
    }
    
    if (bIsInCover)
    {
        return false;
    }
    
    return true;
}

bool ABlackRemasteredCharacter::CanCrouch() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting || CharacterState == CS_Mantling)
    {
        return false;
    }
    
    if (bIsProne)
    {
        return false;
    }
    
    return true;
}

bool ABlackRemasteredCharacter::CanProne() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting || CharacterState == CS_Mantling)
    {
        return false;
    }
    
    return true;
}

bool ABlackRemasteredCharacter::CanSlide() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting || CharacterState == CS_Mantling)
    {
        return false;
    }
    
    if (bIsCrouching || bIsProne || bIsSliding)
    {
        return false;
    }
    
    if (GetVelocity().Size() < 100.0f)
    {
        return false;
    }
    
    if (CurrentStamina < SlideStaminaCost)
    {
        return false;
    }
    
    if (GetCharacterMovement()->IsFalling())
    {
        return false;
    }
    
    return true;
}

bool ABlackRemasteredCharacter::CanMantle() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting)
    {
        return false;
    }
    
    // Check if there's a ledge to mantle
    if (MovementComponent)
    {
        return MovementComponent->CanMantle();
    }
    
    return false;
}

bool ABlackRemasteredCharacter::CanLean() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting || CharacterState == CS_Mantling)
    {
        return false;
    }
    
    // Check if near cover
    if (CoverComponent)
    {
        return CoverComponent->CanLean();
    }
    
    return false;
}

bool ABlackRemasteredCharacter::CanJump() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting || CharacterState == CS_Mantling)
    {
        return false;
    }
    
    if (bIsInCover)
    {
        return false;
    }
    
    return Super::CanJump();
}

// Cover system
void ABlackRemasteredCharacter::CheckForCover()
{
    if (CoverComponent)
    {
        CoverComponent->CheckForCover();
    }
}

void ABlackRemasteredCharacter::EnterCover()
{
    SetCharacterState(CS_InCover);
}

void ABlackRemasteredCharacter::ExitCover()
{
    SetCharacterState(CS_Idle);
}

// Timer callbacks
void ABlackRemasteredCharacter::OnSprintTimer()
{
    // Sprint timer expired
}

void ABlackRemasteredCharacter::OnSlideTimer()
{
    SetCharacterState(CS_Idle);
    bIsSliding = false;
}

void ABlackRemasteredCharacter::OnMantleTimer()
{
    SetCharacterState(CS_Idle);
}

void ABlackRemasteredCharacter::OnSlideCooldownTimer()
{
    // Slide cooldown expired
}

// Damage
void ABlackRemasteredCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (HealthComponent)
    {
        HealthComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    }
    
    // Broadcast damage taken
    OnDamageTaken.Broadcast(Damage, EventInstigator, DamageCauser);
    
    // Update game state stats
    if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
    {
        GameState->AddShot(true); // Enemy hit player
    }
}

// Death
void ABlackRemasteredCharacter::Die(AController* Killer)
{
    SetCharacterState(CS_Dead);
    
    // Disable movement
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
    }
    
    // Disable collision
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    
    // Broadcast death
    OnDeath.Broadcast(Killer);
    
    // Update game state stats
    if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
    {
        GameState->AddDeath();
    }
    
    // Handle game over
    if (ABlackRemasteredGameMode* GameMode = GetWorld()->GetAuthGameMode<ABlackRemasteredGameMode>())
    {
        if (GameMode->bPermadeathEnabled || GameMode->bIronmanMode)
        {
            GameMode->SetGameState(GS_GameOver);
        }
    }
}

// Respawn
void ABlackRemasteredCharacter::Respawn()
{
    // Reset state
    SetCharacterState(CS_Idle);
    bIsAiming = false;
    bIsSprinting = false;
    bIsCrouching = false;
    bIsProne = false;
    bIsSliding = false;
    bIsInCover = false;
    
    // Reset health
    if (HealthComponent)
    {
        HealthComponent->ResetHealth();
    }
    
    // Enable movement
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
    
    // Enable collision
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
    
    // Broadcast respawn
    OnRespawn.Broadcast();
}

// Footstep system
void ABlackRemasteredCharacter::PlayFootstepSound()
{
    // TODO: Implement footstep sound system
    // Different sounds for different surfaces
    // Volume based on character state (running vs walking)
}
