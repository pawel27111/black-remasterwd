#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlackRemastered.h"
#include "BlackRemasteredCharacter.generated.h"

class UBlackRemasteredWeaponComponent;
class UBlackRemasteredHealthComponent;
class UBlackRemasteredMovementComponent;
class UBlackRemasteredInventoryComponent;
class UBlackRemasteredInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UBlackRemasteredCoverComponent;

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABlackRemasteredCharacter();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;
    virtual void Landed(const FHitResult& Hit) override;
    
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredWeaponComponent* WeaponComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredHealthComponent* HealthComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredMovementComponent* MovementComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredInventoryComponent* InventoryComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredInteractionComponent* InteractionComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredCoverComponent* CoverComponent;
    
    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Camera")
    USpringArmComponent* CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Camera")
    UCameraComponent* FollowCamera;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    ECharacterState CharacterState;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    ELeanDirection LeanDirection;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsAiming;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsSprinting;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsCrouching;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsProne;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsSliding;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsInCover;
    
    // Movement settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float WalkSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float RunSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float CrouchSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float ProneSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float SprintSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float SlideSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float MantleSpeed;
    
    // Camera settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraFOV;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float AimFOV;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraLagSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraLagMaxDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    FVector CameraOffset;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    FVector AimCameraOffset;
    
    // Input bindings
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void MoveForward(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void MoveRight(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void LookUp(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void LookRight(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartSprint();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopSprint();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartCrouch();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopCrouch();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartProne();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopProne();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartSlide();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartAim();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopAim();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartLeanLeft();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopLeanLeft();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartLeanRight();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopLeanRight();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Jump();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Mantle();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Interact();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Reload();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Fire();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Aim();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Melee();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void ThrowGrenade();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void NextWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void PreviousWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void SelectWeapon(int32 Index);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void ToggleFireMode();
    
    // State management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|State")
    void SetCharacterState(ECharacterState NewState);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|State")
    void SetLeanDirection(ELeanDirection NewDirection);
    
    // Camera management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Camera")
    void UpdateCamera(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Camera")
    void ApplyCameraEffects(float DeltaTime);
    
    // Movement helpers
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanSprint() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanCrouch() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanProne() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanSlide() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanMantle() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanLean() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanJump() const;
    
    // Cover system
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void CheckForCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void EnterCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void ExitCover();
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Cover")
    bool IsInCover() const { return bIsInCover; }
    
    // Damage
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Damage")
    void TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
    
    // Death
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Death")
    void Die(AController* Killer);
    
    // Respawn
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Respawn")
    void Respawn();
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredHealthComponent* GetHealthComponent() const { return HealthComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredMovementComponent* GetMovementComponent() const { return MovementComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredCoverComponent* GetCoverComponent() const { return CoverComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsAiming() const { return bIsAiming; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsSprinting() const { return bIsSprinting; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsCrouching() const { return bIsCrouching; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsProne() const { return bIsProne; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsSliding() const { return bIsSliding; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    ECharacterState GetCharacterState() const { return CharacterState; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    ELeanDirection GetLeanDirection() const { return LeanDirection; }
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterStateChanged, ECharacterState, OldState, ECharacterState, NewState);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnCharacterStateChanged OnCharacterStateChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLeanDirectionChanged, ELeanDirection, OldDirection, ELeanDirection, NewDirection);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnLeanDirectionChanged OnLeanDirectionChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageTaken, float, DamageAmount, AController*, DamageInstigator, AActor*, DamageCauser);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnDamageTaken OnDamageTaken;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AController*, Killer);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnDeath OnDeath;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawn);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnRespawn OnRespawn;
    
protected:
    // Input component
    UPROPERTY()
    UInputComponent* InputComponent;
    
    // Movement input
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    float MoveForwardValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    float MoveRightValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    float LookUpValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    float LookRightValue;
    
    // Action input
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bSprintInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bCrouchInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bProneInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bAimInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bLeanLeftInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bLeanRightInput;
    
    // Timers
    UPROPERTY()
    FTimerHandle SprintTimerHandle;
    
    UPROPERTY()
    FTimerHandle SlideTimerHandle;
    
    UPROPERTY()
    FTimerHandle MantleTimerHandle;
    
    UPROPERTY()
    FTimerHandle SlideCooldownTimerHandle;
    
    // Stamina
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float MaxStamina;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Stamina")
    float CurrentStamina;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float StaminaRegenRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float StaminaRegenDelay;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float SprintStaminaCost;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float SlideStaminaCost;
    
    // Camera effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraSwayAmount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraSwaySpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraBobAmount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraBobSpeed;
    
    // Slide settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float SlideDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float SlideCooldown;
    
    // Helper functions
    void UpdateMovement(float DeltaTime);
    void UpdateStamina(float DeltaTime);
    void UpdateState(float DeltaTime);
    
    void OnSprintTimer();
    void OnSlideTimer();
    void OnMantleTimer();
    void OnSlideCooldownTimer();
    
    void ApplyMovementPenalties();
    void ApplyCameraSway(float DeltaTime);
    void ApplyCameraBob(float DeltaTime);
    
    // Lean system
    void UpdateLean(float DeltaTime);
    
    // Animation
    void UpdateAnimation();
    
    // Footstep system
    void PlayFootstepSound();
    
    float LastFootstepTime;
    float FootstepInterval;
};
