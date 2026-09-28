#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackRemastered.h"
#include "BlackRemasteredMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementStateChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlideStarted, float, SlideDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlideEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMantleStarted, float, MantleDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMantleEnded);

class ABlackRemasteredCharacter;
class UCharacterMovementComponent;

UENUM(BlueprintType)
enum class EMovementState : uint8
{
    MS_Walking UMETA(DisplayName = "Walking"),
    MS_Running UMETA(DisplayName = "Running"),
    MS_Crouching UMETA(DisplayName = "Crouching"),
    MS_Prone UMETA(DisplayName = "Prone"),
    MS_Sliding UMETA(DisplayName = "Sliding"),
    MS_Mantling UMETA(DisplayName = "Mantling"),
    MS_InCover UMETA(DisplayName = "In Cover"),
    MS_Leaning UMETA(DisplayName = "Leaning"),
    MS_Jumping UMETA(DisplayName = "Jumping"),
    MS_Falling UMETA(DisplayName = "Falling"),
    MS_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMantleState : uint8
{
    MANTLE_None UMETA(DisplayName = "None"),
    MANTLE_Start UMETA(DisplayName = "Start"),
    MANTLE_Climb UMETA(DisplayName = "Climb"),
    MANTLE_End UMETA(DisplayName = "End"),
    MANTLE_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ESlideState : uint8
{
    SLIDE_None UMETA(DisplayName = "None"),
    SLIDE_Start UMETA(DisplayName = "Start"),
    SLIDE_Sliding UMETA(DisplayName = "Sliding"),
    SLIDE_End UMETA(DisplayName = "End"),
    SLIDE_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMovementSettings
{
    GENERATED_BODY()
    
    // Speeds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
    float WalkSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
    float RunSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
    float SprintSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
    float CrouchSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
    float ProneSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
    float SlideSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
    float MantleSpeed;
    
    // Jump
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Jump")
    float JumpZVelocity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Jump")
    float GravityScale;
    
    // Slide
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Slide")
    float SlideDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Slide")
    float SlideCooldown;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Slide")
    float SlideStaminaCost;
    
    // Mantle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Mantle")
    float MantleDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Mantle")
    float MantleHeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Mantle")
    float MantleStaminaCost;
    
    // Cover
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Cover")
    float CoverSnapDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Cover")
    float CoverMoveSpeed;
    
    // Lean
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Lean")
    float LeanDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Lean")
    float LeanSpeed;
    
    // Acceleration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Acceleration")
    float GroundFriction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Acceleration")
    float MaxAcceleration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Acceleration")
    float BrakingDeceleration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Acceleration")
    float BrakingFriction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Acceleration")
    float RotationalAcceleration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Acceleration")
    float BrakingRotation;
};

USTRUCT(BlueprintType)
struct FSlideData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Slide")
    float CurrentSlideTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Slide")
    float SlideCooldownTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Slide")
    bool bCanSlide;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Slide")
    bool bIsSliding;
};

USTRUCT(BlueprintType)
struct FMantleData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Mantle")
    float CurrentMantleTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Mantle")
    EMantleState MantleState;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Mantle")
    FVector MantleStartLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Mantle")
    FVector MantleTargetLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Mantle")
    bool bIsMantling;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKREMASTERED_API UBlackRemasteredMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBlackRemasteredMovementComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void Initialize(ABlackRemasteredCharacter* OwnerCharacter);
    
    // Movement
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void UpdateMovement(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void UpdateRotation(float DeltaTime);
    
    // Slide
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void StartSlide();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void StopSlide();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanSlide() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsSliding() const { return SlideData.bIsSliding; }
    
    // Mantle
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void StartMantle();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void StopMantle();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanMantle() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsMantling() const { return MantleData.bIsMantling; }
    
    // Cover
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void EnterCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void ExitCover();
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsInCover() const;
    
    // Lean
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void StartLean(ELeanDirection Direction);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void StopLean();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    void UpdateLean(float DeltaTime);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsLeaning() const { return CurrentLeanDirection != LD_None; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    ELeanDirection GetLeanDirection() const { return CurrentLeanDirection; }
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    EMovementState GetMovementState() const { return CurrentMovementState; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    float GetCurrentSpeed() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    float GetMaxSpeed() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsMoving() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsSprinting() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsCrouching() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsProne() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsJumping() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Movement")
    bool IsFalling() const;
    
    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    FMovementSettings MovementSettings;
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Movement")
    FOnMovementStateChanged OnMovementStateChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Movement")
    FOnSlideStarted OnSlideStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Movement")
    FOnSlideEnded OnSlideEnded;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Movement")
    FOnMantleStarted OnMantleStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Movement")
    FOnMantleEnded OnMantleEnded;
    
protected:
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // Character movement component
    UPROPERTY()
    UCharacterMovementComponent* CharacterMovement;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Movement")
    EMovementState CurrentMovementState;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Movement")
    ELeanDirection CurrentLeanDirection;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Movement")
    float CurrentLeanAmount;
    
    // Slide data
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Movement")
    FSlideData SlideData;
    
    // Mantle data
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Movement")
    FMantleData MantleData;
    
    // Timers
    UPROPERTY()
    FTimerHandle SlideTimerHandle;
    
    UPROPERTY()
    FTimerHandle SlideCooldownTimerHandle;
    
    UPROPERTY()
    FTimerHandle MantleTimerHandle;
    
    // Helper functions
    void UpdateMovementState();
    void UpdateSpeed();
    void UpdateGroundMovement();
    void UpdateAirMovement();
    
    void OnSlideTimer();
    void OnSlideCooldownTimer();
    void OnMantleTimer();
    
    bool CheckForLedge() const;
    FVector FindLedgeLocation() const;
    
    void ApplySlidePhysics();
    void ApplyMantleMovement(float DeltaTime);
    void ApplyLeanOffset(float DeltaTime);
    
    // Cover detection
    void DetectCover();
    bool FindCoverLocation(FVector& CoverLocation, FVector& CoverNormal) const;
    
    // Input tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Movement")
    FVector CurrentVelocity;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Movement")
    float CurrentSpeed;
};
