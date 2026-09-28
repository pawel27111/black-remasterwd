#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackRemastered.h"
#include "BlackRemasteredDestructibleActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UBoxComponent;
class UChaosPhysicsComponent;
class UChaosSolversComponent;
class UParticleSystem;
class USoundCue;

UENUM(BlueprintType)
enum class EDestructibleType : uint8
{
    DT_Wood UMETA(DisplayName = "Wood"),
    DT_Concrete UMETA(DisplayName = "Concrete"),
    DT_Metal UMETA(DisplayName = "Metal"),
    DT_Glass UMETA(DisplayName = "Glass"),
    DT_Plaster UMETA(DisplayName = "Plaster"),
    DT_Brick UMETA(DisplayName = "Brick"),
    DT_Fabric UMETA(DisplayName = "Fabric"),
    DT_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDestructibleState : uint8
{
    DS_Intact UMETA(DisplayName = "Intact"),
    DS_Damaged UMETA(DisplayName = "Damaged"),
    DS_Broken UMETA(DisplayName = "Broken"),
    DS_Destroyed UMETA(DisplayName = "Destroyed"),
    DS_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
    DAM_Ballistic UMETA(DisplayName = "Ballistic"),
    DAM_Explosive UMETA(DisplayName = "Explosive"),
    DAM_Fire UMETA(DisplayName = "Fire"),
    DAM_Melee UMETA(DisplayName = "Melee"),
    DAM_Vehicle UMETA(DisplayName = "Vehicle"),
    DAM_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FDestructibleData
{
    GENERATED_BODY()
    
    // Basic info
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Info")
    FName DestructibleName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Info")
    EDestructibleType DestructibleType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Info")
    EDestructionTier DestructionTier;
    
    // Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Health")
    float MaxHealth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Health")
    float CurrentHealth;
    
    // Damage modifiers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Damage")
    float BallisticDamageMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Damage")
    float ExplosiveDamageMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Damage")
    float FireDamageMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Damage")
    float MeleeDamageMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Damage")
    float VehicleDamageMultiplier;
    
    // Destruction thresholds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Thresholds")
    float DamagedThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Thresholds")
    float BrokenThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Thresholds")
    float DestroyedThreshold;
    
    // Physics
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Physics")
    bool bUseChaosPhysics;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Physics")
    float Mass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Physics")
    float LinearDamping;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Physics")
    float AngularDamping;
    
    // Fracture
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Fracture")
    bool bFractureOnDestruction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Fracture")
    int32 FracturePiecesCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Fracture")
    float FractureImpulseScale;
    
    // Debris
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Debris")
    TArray<UStaticMeshComponent*> DebrisMeshes;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Debris")
    int32 MinDebrisCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Debris")
    int32 MaxDebrisCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Debris")
    float DebrisVelocityScale;
    
    // Chain reactions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|ChainReaction")
    bool bCanTriggerChainReaction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|ChainReaction")
    float ChainReactionRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|ChainReaction")
    float ChainReactionDamage;
    
    // Explosion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Explosion")
    bool bExplodeOnDestruction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Explosion")
    float ExplosionRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Explosion")
    float ExplosionDamage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Explosion")
    float ExplosionForce;
    
    // Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Effects")
    UParticleSystem* DestructionEffect;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Effects")
    USoundCue* DestructionSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Effects")
    UParticleSystem* DamageEffect;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Effects")
    USoundCue* DamageSound;
    
    // XP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|XP")
    int32 DestructionXP;
};

USTRUCT(BlueprintType)
struct FDestructibleChunk
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Chunk")
    UStaticMeshComponent* Mesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Chunk")
    FVector OriginalLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Chunk")
    FRotator OriginalRotation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Chunk")
    bool bIsFractured;
};

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredDestructibleActor : public AActor
{
    GENERATED_BODY()

public:
    ABlackRemasteredDestructibleActor();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void Initialize(const FDestructibleData& Data);
    
    // Damage
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyDamage(float Damage, EDamageType DamageType, FVector HitLocation, FVector HitNormal);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyBallisticDamage(float Damage, FVector HitLocation, FVector HitNormal);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyExplosiveDamage(float Damage, FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyFireDamage(float Damage, FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyMeleeDamage(float Damage, FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyVehicleDamage(float Damage, FVector HitLocation);
    
    // Destruction
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void Destroy();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void Fracture();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void SpawnDebris();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void TriggerChainReaction();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void Explode();
    
    // State
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void SetDestructibleState(EDestructibleState NewState);
    
    // Repair
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void Repair(float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void FullyRepair();
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    EDestructibleType GetDestructibleType() const { return DestructibleData.DestructibleType; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    EDestructionTier GetDestructionTier() const { return DestructibleData.DestructionTier; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    EDestructibleState GetDestructibleState() const { return CurrentState; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    float GetCurrentHealth() const { return DestructibleData.CurrentHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    float GetMaxHealth() const { return DestructibleData.MaxHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    float GetHealthPercentage() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    bool IsIntact() const { return CurrentState == DS_Intact; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    bool IsDamaged() const { return CurrentState == DS_Damaged; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    bool IsBroken() const { return CurrentState == DS_Broken; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    bool IsDestroyed() const { return CurrentState == DS_Destroyed; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    bool CanBeDestroyed() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    bool CanTriggerChainReaction() const;
    
    // Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Destruction")
    FDestructibleData DestructibleData;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageApplied, float, DamageAmount, EDamageType, DamageType, FVector, HitLocation);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Destruction")
    FOnDamageApplied OnDamageApplied;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChanged, EDestructibleState, OldState, EDestructibleState, NewState);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Destruction")
    FOnStateChanged OnStateChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyed);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Destruction")
    FOnDestroyed OnDestroyed;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChainReactionTriggered);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Destruction")
    FOnChainReactionTriggered OnChainReactionTriggered;
    
protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Destruction")
    UStaticMeshComponent* MeshComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Destruction")
    UChaosPhysicsComponent* ChaosPhysicsComponent;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Destruction")
    EDestructibleState CurrentState;
    
    // Chunks
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Destruction")
    TArray<FDestructibleChunk> Chunks;
    
    // Debris
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Destruction")
    TArray<AActor*> SpawnedDebris;
    
    // Timers
    UPROPERTY()
    FTimerHandle ChainReactionTimerHandle;
    
    // Helper functions
    void OnChainReactionTimer();
    
    void UpdateState();
    
    void SpawnChunk(FDestructibleChunk& ChunkData);
    
    void ApplyDamageEffects(float Damage, EDamageType DamageType, FVector HitLocation);
    
    void ApplyDestructionEffects();
    
    void ApplyDamageDecals(float Damage, EDamageType DamageType, FVector HitLocation, FVector HitNormal);
    
    void CheckForChainReaction();
    
    void ApplyExplosionForce(FVector ExplosionLocation, float Force);
    
    bool IsSupportingStructure() const;
};
