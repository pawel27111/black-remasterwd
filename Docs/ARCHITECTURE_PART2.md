# BLACK: REMASTERED - TECHNICAL ARCHITECTURE DOCUMENT (PART 2)
## Core Systems Implementation

---

## 4. GAMEPLAY SYSTEMS (CONTINUED)

### 4.1 Player Character (CONTINUED)

#### 4.1.2 BlackRemasteredCharacter.cpp
```cpp
#include "BlackRemasteredCharacter.h"
#include "BlackRemasteredWeaponComponent.h"
#include "BlackRemasteredHealthComponent.h"
#include "BlackRemasteredMovementComponent.h"
#include "BlackRemasteredInventoryComponent.h"
#include "BlackRemasteredInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

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
    WalkSpeed = 200.0f;
    RunSpeed = 300.0f;
    CrouchSpeed = 150.0f;
    ProneSpeed = 100.0f;
    SprintSpeed = 400.0f;
    SlideSpeed = 500.0f;
    MantleSpeed = 200.0f;
    
    // Camera settings
    CameraFOV = 90.0f;
    AimFOV = 60.0f;
    CameraLagSpeed = 10.0f;
    CameraLagMaxDistance = 100.0f;
    CameraOffset = FVector(0.0f, 0.0f, 60.0f);
    AimCameraOffset = FVector(0.0f, 0.0f, 50.0f);
    
    // Camera sway settings
    CameraSwayAmount = 5.0f;
    CameraSwaySpeed = 2.0f;
    CameraBobAmount = 2.0f;
    CameraBobSpeed = 3.0f;
    
    // Stamina settings
    MaxStamina = 100.0f;
    CurrentStamina = MaxStamina;
    StaminaRegenRate = 20.0f;
    StaminaRegenDelay = 1.0f;
    SprintStaminaCost = 10.0f;
    SlideStaminaCost = 20.0f;
    
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
    
    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->GravityScale = 1.0f;
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
    }
}

void ABlackRemasteredCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    InputComponent = PlayerInputComponent;
    
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
    InputComponent->BindAction("Aim", IE_Pressed, this, &ABlackRemasteredCharacter::Aim);
    InputComponent->BindAction("Melee", IE_Pressed, this, &ABlackRemasteredCharacter::Melee);
    InputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ABlackRemasteredCharacter::ThrowGrenade);
    InputComponent->BindAction("NextWeapon", IE_Pressed, this, &ABlackRemasteredCharacter::NextWeapon);
    InputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &ABlackRemasteredCharacter::PreviousWeapon);
    
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
            1.0f,
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
            bIsSprinting = false;
            break;
            
        case CS_Mantling:
            bIsSprinting = false;
            break;
            
        case CS_InCover:
            bIsSprinting = false;
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
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting)
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
    else if (bIsCrouching)
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
        }
    }
    else if (bIsProne)
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
    CheckForCover();
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
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting)
    {
        return false;
    }
    
    if (bIsCrouching || bIsProne)
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
    
    return true;
}

bool ABlackRemasteredCharacter::CanCrouch() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting)
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
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting)
    {
        return false;
    }
    
    if (bIsCrouching)
    {
        return true;
    }
    
    return true;
}

bool ABlackRemasteredCharacter::CanSlide() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting)
    {
        return false;
    }
    
    if (bIsCrouching || bIsProne)
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
    
    return true;
}

bool ABlackRemasteredCharacter::CanMantle() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting)
    {
        return false;
    }
    
    // Check if there's a ledge to mantle
    // TODO: Implement ledge detection
    
    return true;
}

bool ABlackRemasteredCharacter::CanLean() const
{
    if (CharacterState == CS_Dead || CharacterState == CS_Interacting)
    {
        return false;
    }
    
    // Check if near cover
    // TODO: Implement cover detection
    
    return true;
}

// Cover system
void ABlackRemasteredCharacter::CheckForCover()
{
    // TODO: Implement cover detection
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
}

void ABlackRemasteredCharacter::OnMantleTimer()
{
    SetCharacterState(CS_Idle);
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
```

---

## 4.2 Player Controller

### 4.2.1 BlackRemasteredPlayerController.h
```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlackRemasteredPlayerController.generated.h"

class ABlackRemasteredCharacter;
class ABlackRemasteredGameMode;
class UBlackRemasteredHUD;

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ABlackRemasteredPlayerController();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Player")
    void InitializePlayer();
    
    // HUD
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|UI")
    UBlackRemasteredHUD* BlackRemasteredHUD;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|UI")
    UBlackRemasteredHUD* GetBlackRemasteredHUD() const { return BlackRemasteredHUD; }
    
    // Input
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void TogglePause();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void OpenMenu();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void OpenInventory();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void OpenMap();
    
    // Game state
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Game")
    void SetGamePaused(bool bPaused);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Game")
    bool IsGamePaused() const;
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    ABlackRemasteredCharacter* GetBlackRemasteredCharacter() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    ABlackRemasteredGameMode* GetBlackRemasteredGameMode() const;
    
protected:
    // Input bindings
    virtual void BindInputActions();
    
    // Pause state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Game")
    bool bIsPaused;
    
    // UI state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|UI")
    bool bIsMenuOpen;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|UI")
    bool bIsInventoryOpen;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|UI")
    bool bIsMapOpen;
    
    // Input component
    UPROPERTY()
    UInputComponent* InputComponent;
};
```

#### 4.2.2 BlackRemasteredPlayerController.cpp
```cpp
#include "BlackRemasteredPlayerController.h"
#include "BlackRemasteredCharacter.h"
#include "BlackRemasteredGameMode.h"
#include "BlackRemasteredHUD.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

ABlackRemasteredPlayerController::ABlackRemasteredPlayerController()
    : Super()
{
    bIsPaused = false;
    bIsMenuOpen = false;
    bIsInventoryOpen = false;
    bIsMapOpen = false;
}

void ABlackRemasteredPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize player
    InitializePlayer();
}

void ABlackRemasteredPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABlackRemasteredPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    InputComponent = GetInputComponent();
    
    if (InputComponent)
    {
        BindInputActions();
    }
}

void ABlackRemasteredPlayerController::InitializePlayer()
{
    // Create HUD
    if (!BlackRemasteredHUD)
    {
        BlackRemasteredHUD = NewObject<UBlackRemasteredHUD>(this);
        BlackRemasteredHUD->Initialize(this);
    }
    
    // Set initial game state
    if (ABlackRemasteredGameMode* GameMode = GetBlackRemasteredGameMode())
    {
        GameMode->SetGameState(GS_Playing);
    }
}

void ABlackRemasteredPlayerController::BindInputActions()
{
    if (!InputComponent)
    {
        return;
    }
    
    // Pause
    InputComponent->BindAction("Pause", IE_Pressed, this, &ABlackRemasteredPlayerController::TogglePause);
    
    // Menu
    InputComponent->BindAction("Menu", IE_Pressed, this, &ABlackRemasteredPlayerController::OpenMenu);
    
    // Inventory
    InputComponent->BindAction("Inventory", IE_Pressed, this, &ABlackRemasteredPlayerController::OpenInventory);
    
    // Map
    InputComponent->BindAction("Map", IE_Pressed, this, &ABlackRemasteredPlayerController::OpenMap);
}

void ABlackRemasteredPlayerController::TogglePause()
{
    bIsPaused = !bIsPaused;
    SetGamePaused(bIsPaused);
    
    if (bIsPaused)
    {
        // Open pause menu
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->ShowPauseMenu();
        }
    }
    else
    {
        // Close pause menu
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->HidePauseMenu();
        }
    }
}

void ABlackRemasteredPlayerController::OpenMenu()
{
    bIsMenuOpen = !bIsMenuOpen;
    
    if (bIsMenuOpen)
    {
        // Open main menu
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->ShowMainMenu();
        }
    }
    else
    {
        // Close main menu
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->HideMainMenu();
        }
    }
}

void ABlackRemasteredPlayerController::OpenInventory()
{
    bIsInventoryOpen = !bIsInventoryOpen;
    
    if (bIsInventoryOpen)
    {
        // Open inventory
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->ShowInventory();
        }
        
        // Pause game
        SetGamePaused(true);
    }
    else
    {
        // Close inventory
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->HideInventory();
        }
        
        // Resume game
        SetGamePaused(false);
    }
}

void ABlackRemasteredPlayerController::OpenMap()
{
    bIsMapOpen = !bIsMapOpen;
    
    if (bIsMapOpen)
    {
        // Open map
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->ShowMap();
        }
        
        // Pause game
        SetGamePaused(true);
    }
    else
    {
        // Close map
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->HideMap();
        }
        
        // Resume game
        SetGamePaused(false);
    }
}

void ABlackRemasteredPlayerController::SetGamePaused(bool bPaused)
{
    bIsPaused = bPaused;
    UGameplayStatics::SetGamePaused(GetWorld(), bPaused);
    
    if (ABlackRemasteredGameMode* GameMode = GetBlackRemasteredGameMode())
    {
        if (bPaused)
        {
            GameMode->SetGameState(GS_Paused);
        }
        else
        {
            GameMode->SetGameState(GS_Playing);
        }
    }
}

bool ABlackRemasteredPlayerController::IsGamePaused() const
{
    return bIsPaused;
}

ABlackRemasteredCharacter* ABlackRemasteredPlayerController::GetBlackRemasteredCharacter() const
{
    if (APawn* Pawn = GetPawn())
    {
        return Cast<ABlackRemasteredCharacter>(Pawn);
    }
    return nullptr;
}

ABlackRemasteredGameMode* ABlackRemasteredPlayerController::GetBlackRemasteredGameMode() const
{
    if (UWorld* World = GetWorld())
    {
        return Cast<ABlackRemasteredGameMode>(World->GetAuthGameMode());
    }
    return nullptr;
}
```

---

## 4.3 Component System

### 4.3.1 Health Component

#### 4.3.1.1 BlackRemasteredHealthComponent.h
```cpp
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackRemasteredHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth, float, DamageAmount, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthRestored);

class ABlackRemasteredCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKREMASTERED_API UBlackRemasteredHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBlackRemasteredHealthComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void Initialize(ABlackRemasteredCharacter* OwnerCharacter);
    
    // Health management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void Heal(float Amount, AActor* Healer);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void ResetHealth();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void AddArmor(float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void RemoveArmor(float Amount);
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetCurrentHealth() const { return CurrentHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetMaxHealth() const { return MaxHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetHealthPercentage() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetCurrentArmor() const { return CurrentArmor; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetMaxArmor() const { return MaxArmor; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetArmorPercentage() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    bool IsAlive() const { return CurrentHealth > 0.0f; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    bool IsFullHealth() const { return CurrentHealth >= MaxHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    bool HasArmor() const { return CurrentArmor > 0.0f; }
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Health")
    FOnHealthChanged OnHealthChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Health")
    FOnHealthDepleted OnHealthDepleted;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Health")
    FOnHealthRestored OnHealthRestored;
    
protected:
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // Health settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float MaxHealth;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Health")
    float CurrentHealth;
    
    // Armor settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float MaxArmor;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Health")
    float CurrentArmor;
    
    // Regeneration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    bool bRegenerateHealth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float HealthRegenRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float HealthRegenDelay;
    
    // Damage modifiers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float HeadshotDamageMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float ArmorDamageReduction;
    
    // Timers
    UPROPERTY()
    FTimerHandle HealthRegenTimerHandle;
    
    UPROPERTY()
    FTimerHandle LastDamageTimerHandle;
    
    // Helper functions
    void StartHealthRegen();
    void StopHealthRegen();
    void OnHealthRegenTimer();
    void OnLastDamageTimer();
    
    // Damage calculation
    float CalculateActualDamage(float Damage, FDamageEvent const& DamageEvent);
};
```

This is the continuation of the architecture document. Due to the massive scope, I'll now create the actual source code files in the proper directory structure.
