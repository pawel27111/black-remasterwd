#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackRemastered.h"
#include "BlackRemasteredGrenade.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundCue;
class ABlackRemasteredCharacter;

UENUM(BlueprintType)
enum class EGrenadeType : uint8
{
    GT_Fragmentation UMETA(DisplayName = "Fragmentation"),
    GT_Stun UMETA(DisplayName = "Stun"),
    GT_Incendiary UMETA(DisplayName = "Incendiary"),
    GT_Smoke UMETA(DisplayName = "Smoke"),
    GT_Flashbang UMETA(DisplayName = "Flashbang"),
    GT_Molotov UMETA(DisplayName = "Molotov"),
    GT_C4 UMETA(DisplayName = "C4"),
    GT_Mine UMETA(DisplayName = "Mine"),
    GT_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EGrenadeState : uint8
{
    GS_Held UMETA(DisplayName = "Held"),
    GS_Thrown UMETA(DisplayName = "Thrown"),
    GS_Armed UMETA(DisplayName = "Armed"),
    GS_Exploding UMETA(DisplayName = "Exploding"),
    GS_Detonated UMETA(DisplayName = "Detonated"),
    GS_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FGrenadeData
{
    GENERATED_BODY()
    
    // Basic info
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Info")
    FName GrenadeName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Info")
    EGrenadeType GrenadeType;
    
    // Visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Visuals")
    UStaticMeshComponent* Mesh;
    
    // Explosion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Explosion")
    float ExplosionRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Explosion")
    float ExplosionDamage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Explosion")
    bool bHurtsOwner;
    
    // Timing
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Timing")
    float FuseTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Timing")
    float ArmTime;
    
    // Physics
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Physics")
    float ThrowForce;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Physics")
    float MaxBounces;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Physics")
    float BounceDamping;
    
    // Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Effects")
    UParticleSystem* ExplosionEffect;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Effects")
    USoundCue* ExplosionSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Effects")
    USoundCue* BounceSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Effects")
    USoundCue* ArmSound;
    
    // Camera effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Effects")
    TSubclassOf<UCameraShake> ExplosionCameraShake;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade|Effects")
    float CameraShakeScale;
};

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredGrenade : public AActor
{
    GENERATED_BODY()

public:
    ABlackRemasteredGrenade();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Grenade")
    void Initialize(const FGrenadeData& Data);
    
    // Throw
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Grenade")
    void Throw(ABlackRemasteredCharacter* Thrower);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Grenade")
    void Detonate();
    
    // State
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Grenade")
    void SetGrenadeState(EGrenadeState NewState);
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Grenade")
    EGrenadeType GetGrenadeType() const { return GrenadeData.GrenadeType; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Grenade")
    EGrenadeState GetGrenadeState() const { return CurrentState; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Grenade")
    float GetFuseTime() const { return GrenadeData.FuseTime; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Grenade")
    float GetExplosionRadius() const { return GrenadeData.ExplosionRadius; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Grenade")
    float GetExplosionDamage() const { return GrenadeData.ExplosionDamage; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Grenade")
    bool IsArmed() const { return CurrentState == GS_Armed || CurrentState == GS_Exploding; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Grenade")
    bool HasExploded() const { return CurrentState == GS_Detonated; }
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrenadeThrown);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Grenade")
    FOnGrenadeThrown OnGrenadeThrown;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrenadeArmed);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Grenade")
    FOnGrenadeArmed OnGrenadeArmed;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrenadeExploded);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Grenade")
    FOnGrenadeExploded OnGrenadeExploded;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrenadeBounced);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Grenade")
    FOnGrenadeBounced OnGrenadeBounced;
    
protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Grenade")
    USphereComponent* CollisionComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Grenade")
    UStaticMeshComponent* MeshComponent;
    
    // Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Grenade")
    FGrenadeData GrenadeData;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Grenade")
    EGrenadeState CurrentState;
    
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // Timers
    UPROPERTY()
    FTimerHandle FuseTimerHandle;
    
    UPROPERTY()
    FTimerHandle ArmTimerHandle;
    
    // Bounce tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Grenade")
    int32 BounceCount;
    
    // Helper functions
    void OnFuseTimer();
    void OnArmTimer();
    
    void Explode();
    
    void ApplyExplosionDamage();
    void ApplyExplosionForce();
    void SpawnExplosionEffects();
    
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    UFUNCTION()
    void OnBounce(const FHitResult& Hit);
    
    // Physics
    void ApplyPhysicsMaterial(UMaterialInterface* Material);
};
