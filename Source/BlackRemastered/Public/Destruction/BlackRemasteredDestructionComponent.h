#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackRemastered.h"
#include "BlackRemasteredDestructionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestructionApplied, AActor*, DestroyedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChainReactionStarted, AActor*, TriggerActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestructionSystemReady);

class ABlackRemasteredDestructibleActor;
class ABlackRemasteredCharacter;
class UChaosPhysicsComponent;
class UChaosSolversComponent;

UENUM(BlueprintType)
enum class EDestructionMode : uint8
{
    DM_Standard UMETA(DisplayName = "Standard"),
    DM_Voxel UMETA(DisplayName = "Voxel-Based"),
    DM_Fracture UMETA(DisplayName = "Fracture-Based"),
    DM_Hybrid UMETA(DisplayName = "Hybrid"),
    DM_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDestructionQuality : uint8
{
    DQ_Low UMETA(DisplayName = "Low"),
    DQ_Medium UMETA(DisplayName = "Medium"),
    DQ_High UMETA(DisplayName = "High"),
    DQ_Ultra UMETA(DisplayName = "Ultra"),
    DQ_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FDestructionSettings
{
    GENERATED_BODY()
    
    // Mode
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Settings")
    EDestructionMode DestructionMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Settings")
    EDestructionQuality DestructionQuality;
    
    // Performance
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Performance")
    int32 MaxActiveDebris;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Performance")
    int32 MaxFracturePieces;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Performance")
    float DebrisCleanupTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Performance")
    float DebrisCleanupRadius;
    
    // Voxel settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Voxel")
    float VoxelSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Voxel")
    int32 VoxelGridSize;
    
    // Fracture settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Fracture")
    float FractureImpulseScale;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Fracture")
    float FractureMinChunkSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Fracture")
    float FractureMaxChunkSize;
    
    // Chain reaction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|ChainReaction")
    bool bEnableChainReactions;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|ChainReaction")
    float ChainReactionRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|ChainReaction")
    float ChainReactionDelay;
    
    // Damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Damage")
    float MinDamageForDestruction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Damage")
    float MaxDamagePerHit;
    
    // Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Effects")
    bool bEnableDestructionEffects;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Effects")
    bool bEnableDebrisEffects;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Effects")
    bool bEnableSoundEffects;
    
    // Debug
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Debug")
    bool bDebugDestruction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Debug")
    bool bDebugVoxels;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction|Debug")
    bool bDebugFractures;
};

USTRUCT(BlueprintType)
struct FDestructionStats
{
    GENERATED_BODY()
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destruction|Stats")
    int32 TotalDestructibles;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destruction|Stats")
    int32 TotalDestroyed;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destruction|Stats")
    int32 TotalDebrisSpawned;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destruction|Stats")
    int32 TotalChainReactions;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destruction|Stats")
    float TotalDestructionDamage;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKREMASTERED_API UBlackRemasteredDestructionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBlackRemasteredDestructionComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void Initialize(AActor* Owner);
    
    // Destruction
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyDamageToActor(AActor* Actor, float Damage, EDamageType DamageType, FVector HitLocation, FVector HitNormal);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyBallisticDamageToActor(AActor* Actor, float Damage, FVector HitLocation, FVector HitNormal);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyExplosiveDamageToActor(AActor* Actor, float Damage, FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyFireDamageToActor(AActor* Actor, float Damage, FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyMeleeDamageToActor(AActor* Actor, float Damage, FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyVehicleDamageToActor(AActor* Actor, float Damage, FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void DestroyActor(AActor* Actor);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void TriggerChainReaction(AActor* TriggerActor);
    
    // Raycasting
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    bool TraceForDestructible(FVector Start, FVector End, FHitResult& HitResult);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    bool SphereTraceForDestructibles(FVector Center, float Radius, TArray<AActor*>& OutDestructibles);
    
    // Voxel destruction
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyVoxelDestruction(AActor* Actor, FVector HitLocation, float Radius, float Damage);
    
    // Fracture destruction
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplyFractureDestruction(AActor* Actor, FVector HitLocation, float Damage);
    
    // Surface damage
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void ApplySurfaceDamage(AActor* Actor, FVector HitLocation, FVector HitNormal, float Damage);
    
    // Debris
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void SpawnDebris(AActor* Actor, FVector Location, FVector Direction, float Force);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Destruction")
    void CleanupDebris();
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    FDestructionSettings GetDestructionSettings() const { return DestructionSettings; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    FDestructionStats GetDestructionStats() const { return DestructionStats; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    bool IsDestructionEnabled() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    bool CanDestroyActor(AActor* Actor) const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    EDestructionMode GetDestructionMode() const { return DestructionSettings.DestructionMode; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Destruction")
    EDestructionQuality GetDestructionQuality() const { return DestructionSettings.DestructionQuality; }
    
    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Destruction")
    FDestructionSettings DestructionSettings;
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Destruction")
    FOnDestructionApplied OnDestructionApplied;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Destruction")
    FOnChainReactionStarted OnChainReactionStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Destruction")
    FOnDestructionSystemReady OnDestructionSystemReady;
    
protected:
    // Owner
    UPROPERTY()
    AActor* OwnerActor;
    
    // Chaos physics
    UPROPERTY()
    UChaosPhysicsComponent* ChaosPhysicsComponent;
    
    UPROPERTY()
    UChaosSolversComponent* ChaosSolversComponent;
    
    // Stats
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Destruction")
    FDestructionStats DestructionStats;
    
    // Timers
    UPROPERTY()
    FTimerHandle DebrisCleanupTimerHandle;
    
    // Helper functions
    void OnDebrisCleanupTimer();
    
    void InitializeChaosPhysics();
    void InitializeDestructionSettings();
    
    void HandleDestruction(AActor* Actor, float Damage, EDamageType DamageType, FVector HitLocation, FVector HitNormal);
    
    void UpdateDestructionStats(AActor* Actor, float Damage);
    
    void CheckForSupportingStructures(AActor* DestroyedActor);
    
    bool IsActorDestructible(AActor* Actor) const;
    
    EDestructionTier GetActorDestructionTier(AActor* Actor) const;
};
