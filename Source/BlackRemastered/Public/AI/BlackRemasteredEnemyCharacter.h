#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlackRemastered.h"
#include "BlackRemasteredEnemyCharacter.generated.h"

class UBlackRemasteredHealthComponent;
class UBlackRemasteredWeaponComponent;
class ABlackRemasteredCharacter;
class ABlackRemasteredAIController;
class UBehaviorTree;
class UBlackboardComponent;

UENUM(BlueprintType)
enum class EEnemyBehavior : uint8
{
    EB_Idle UMETA(DisplayName = "Idle"),
    EB_Patrol UMETA(DisplayName = "Patrol"),
    EB_Alert UMETA(DisplayName = "Alert"),
    EB_Search UMETA(DisplayName = "Search"),
    EB_Investigate UMETA(DisplayName = "Investigate"),
    EB_Engage UMETA(DisplayName = "Engage"),
    EB_Suppress UMETA(DisplayName = "Suppress"),
    EB_Flank UMETA(DisplayName = "Flank"),
    EB_Retreat UMETA(DisplayName = "Retreat"),
    EB_Regroup UMETA(DisplayName = "Regroup"),
    EB_Cover UMETA(DisplayName = "In Cover"),
    EB_Dead UMETA(DisplayName = "Dead"),
    EB_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEnemyDifficulty : uint8
{
    ED_Recruit UMETA(DisplayName = "Recruit"),
    ED_Veteran UMETA(DisplayName = "Veteran"),
    ED_BlackOps UMETA(DisplayName = "Black Ops"),
    ED_Hardcore UMETA(DisplayName = "Hardcore"),
    ED_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FEnemyData
{
    GENERATED_BODY()
    
    // Basic info
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Info")
    FName EnemyName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Info")
    EEnemyType EnemyType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Info")
    EEnemyDifficulty Difficulty;
    
    // Stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
    float MaxHealth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
    float CurrentHealth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
    float MaxArmor;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
    float CurrentArmor;
    
    // Damage modifiers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Damage")
    float HeadshotDamageMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Damage")
    float ArmorDamageReduction;
    
    // Movement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    float WalkSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    float RunSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    float SprintSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
    float CrouchSpeed;
    
    // Perception
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Perception")
    float SightRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Perception")
    float SightAngle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Perception")
    float HearingRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Perception")
    float MemoryTime;
    
    // Combat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
    float Accuracy;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
    float FireRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
    float ReloadTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
    float BurstCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
    float SuppressionRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
    float FlankDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
    float RetreatHealthThreshold;
    
    // AI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
    float ReactionTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
    float DecisionInterval;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
    float Aggression;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
    float Caution;
    
    // Weapons
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Weapons")
    TArray<EWeaponType> AvailableWeapons;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Weapons")
    EWeaponType CurrentWeaponType;
    
    // Grenades
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Grenades")
    bool bCanUseGrenades;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Grenades")
    float GrenadeCooldown;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Grenades")
    EGrenadeType PreferredGrenadeType;
    
    // Spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    bool bSpawnWithWeapon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    bool bSpawnWithGrenades;
    
    // XP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|XP")
    int32 KillXP;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|XP")
    int32 HeadshotXP;
};

USTRUCT(BlueprintType)
struct FEnemySpawnData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    EEnemyType EnemyType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    int32 SpawnWeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    int32 MinSpawnCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    int32 MaxSpawnCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    float SpawnInterval;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    bool bSpawnOnAlert;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Spawn")
    bool bSpawnOnDeath;
};

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABlackRemasteredEnemyCharacter();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void PossessedBy(AController* NewController) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void Initialize(const FEnemyData& Data);
    
    // Combat
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void Fire();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void Reload();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void ThrowGrenade();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void MeleeAttack();
    
    // Movement
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void MoveToLocation(const FVector& Location);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void MoveToCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void TakeCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void Flank();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void Retreat();
    
    // Perception
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void DetectPlayer();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void LosePlayer();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void HearNoise(const FVector& NoiseLocation, float NoiseLoudness);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void SeeDeadBody(AActor* DeadBody);
    
    // State
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void SetEnemyState(EEnemyState NewState);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void SetEnemyBehavior(EEnemyBehavior NewBehavior);
    
    // Damage
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
    
    // Death
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Enemy")
    void Die(AController* Killer);
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    EEnemyType GetEnemyType() const { return EnemyData.EnemyType; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    EEnemyState GetEnemyState() const { return CurrentState; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    EEnemyBehavior GetEnemyBehavior() const { return CurrentBehavior; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    float GetCurrentHealth() const { return EnemyData.CurrentHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    float GetMaxHealth() const { return EnemyData.MaxHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    float GetHealthPercentage() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    bool IsAlive() const { return CurrentState != ES_Dead; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    bool IsEngaged() const { return CurrentBehavior == EB_Engage || CurrentBehavior == EB_Suppress; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    bool HasLineOfSight() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    bool HasTarget() const { return TargetCharacter != nullptr; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    ABlackRemasteredCharacter* GetTarget() const { return TargetCharacter; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Enemy")
    ABlackRemasteredAIController* GetEnemyAIController() const;
    
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Enemy")
    UBlackRemasteredHealthComponent* HealthComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Enemy")
    UBlackRemasteredWeaponComponent* WeaponComponent;
    
    // Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Enemy")
    FEnemyData EnemyData;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyStateChanged, EEnemyState, OldState, EEnemyState, NewState);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Enemy")
    FOnEnemyStateChanged OnEnemyStateChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyBehaviorChanged, EEnemyBehavior, OldBehavior, EEnemyBehavior, NewBehavior);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Enemy")
    FOnEnemyBehaviorChanged OnEnemyBehaviorChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEnemyTookDamage, float, DamageAmount, AController*, DamageInstigator, AActor*, DamageCauser);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Enemy")
    FOnEnemyTookDamage OnEnemyTookDamage;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, AController*, Killer);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Enemy")
    FOnEnemyDied OnEnemyDied;
    
protected:
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Enemy")
    EEnemyState CurrentState;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Enemy")
    EEnemyBehavior CurrentBehavior;
    
    // Target
    UPROPERTY()
    ABlackRemasteredCharacter* TargetCharacter;
    
    // AI Controller
    UPROPERTY()
    ABlackRemasteredAIController* EnemyAIController;
    
    // Timers
    UPROPERTY()
    FTimerHandle BehaviorTimerHandle;
    
    UPROPERTY()
    FTimerHandle DecisionTimerHandle;
    
    UPROPERTY()
    FTimerHandle PerceptionTimerHandle;
    
    // Helper functions
    void UpdateAI(float DeltaTime);
    void MakeDecision();
    void UpdatePerception(float DeltaTime);
    
    void OnBehaviorTimer();
    void OnDecisionTimer();
    void OnPerceptionTimer();
    
    void FindTarget();
    void UpdateTarget();
    
    void PlayFootstepSound();
    void PlayVoiceLine(EEnemyState State);
    
    // Cover system
    void FindCover();
    bool CanTakeCover() const;
    
    // Flanking
    void FindFlankPosition(FVector& OutPosition);
    
    // Navigation
    void CheckForObstacles();
    void AvoidObstacle();
};
