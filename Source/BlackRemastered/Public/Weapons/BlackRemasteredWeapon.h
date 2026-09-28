#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackRemastered.h"
#include "BlackRemasteredWeapon.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundCue;
class UAnimMontage;
class ABlackRemasteredCharacter;
class ABlackRemasteredProjectile;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    WS_Idle UMETA(DisplayName = "Idle"),
    WS_Firing UMETA(DisplayName = "Firing"),
    WS_Reloading UMETA(DisplayName = "Reloading"),
    WS_Melee UMETA(DisplayName = "Melee"),
    WS_Jammed UMETA(DisplayName = "Jammed"),
    WS_Overheated UMETA(DisplayName = "Overheated"),
    WS_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeaponAttachment : uint8
{
    WA_None UMETA(DisplayName = "None"),
    WA_RedDot UMETA(DisplayName = "Red Dot Sight"),
    WA_ACOG UMETA(DisplayName = "ACOG Scope"),
    WA_SniperScope UMETA(DisplayName = "Sniper Scope"),
    WA_Suppressor UMETA(DisplayName = "Suppressor"),
    WA_ExtendedMag UMETA(DisplayName = "Extended Magazine"),
    WA_Foregrip UMETA(DisplayName = "Foregrip"),
    WA_Bayonet UMETA(DisplayName = "Bayonet"),
    WA_UnderbarrelGL UMETA(DisplayName = "Underbarrel Grenade Launcher"),
    WA_Bipod UMETA(DisplayName = "Bipod"),
    WA_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()
    
    // Basic info
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Info")
    FName WeaponName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Info")
    EWeaponType WeaponType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Info")
    FString Description;
    
    // Visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visuals")
    USkeletalMeshComponent* Mesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Visuals")
    FName AttachSocket;
    
    // Fire modes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Fire")
    EWeaponFireMode DefaultFireMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Fire")
    TArray<EWeaponFireMode> AvailableFireModes;
    
    // Ammo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 MagazineCapacity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 MaxAmmo;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 CurrentAmmo;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 ReserveAmmo;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    EAmmoType AmmoType;
    
    // Damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage")
    float BaseDamage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage")
    float HeadshotMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage")
    float ArmorPenetration;
    
    // Fire rates
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Fire")
    float FireRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Fire")
    float BurstFireRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Fire")
    int32 BurstCount;
    
    // Accuracy
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Accuracy")
    float HipFireSpread;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Accuracy")
    float ADSSpread;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Accuracy")
    float MovementSpread;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Accuracy")
    float JumpingSpread;
    
    // Recoil
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil")
    float VerticalRecoil;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil")
    float HorizontalRecoil;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil")
    float FirstShotAccuracyBonus;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Recoil")
    float RecoilRecoverySpeed;
    
    // Reload
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Reload")
    float ReloadTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Reload")
    float TacticalReloadTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Reload")
    bool bInterruptibleReload;
    
    // Overheating
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Overheat")
    bool bCanOverheat;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Overheat")
    float OverheatThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Overheat")
    float OverheatCooldown;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Overheat")
    float HeatPerShot;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Overheat")
    float HeatCooldownRate;
    
    // Jamming
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Jam")
    bool bCanJam;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Jam")
    float JamProbability;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Jam")
    float JamClearTime;
    
    // Range
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Range")
    float MaxRange;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Range")
    float EffectiveRange;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Range")
    float DamageFalloffStart;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Range")
    float DamageFalloffEnd;
    
    // Projectile
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile")
    bool bUseProjectile;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile")
    TSubclassOf<ABlackRemasteredProjectile> ProjectileClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile")
    float ProjectileSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile")
    float ProjectileGravityScale;
    
    // Sounds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sounds")
    USoundCue* FireSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sounds")
    USoundCue* DryFireSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sounds")
    USoundCue* ReloadSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sounds")
    USoundCue* TacticalReloadSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sounds")
    USoundCue* CockSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sounds")
    USoundCue* OutOfAmmoSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sounds")
    USoundCue* JamSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sounds")
    USoundCue* OverheatSound;
    
    // Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    UParticleSystem* MuzzleFlash;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    UParticleSystem* ShellEjection;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    FName MuzzleFlashSocket;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    FName ShellEjectionSocket;
    
    // Camera effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Camera")
    float CameraShakeScale;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Camera")
    TSubclassOf<UCameraShake> FireCameraShake;
    
    // Animations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animations")
    UAnimMontage* FireAnimation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animations")
    UAnimMontage* ReloadAnimation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animations")
    UAnimMontage* TacticalReloadAnimation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animations")
    UAnimMontage* MeleeAnimation;
    
    // Attachments
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attachments")
    TArray<EWeaponAttachment> AvailableAttachments;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attachments")
    TMap<EWeaponAttachment, USkeletalMeshComponent*> AttachmentMeshes;
    
    // Stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
    int32 KillCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
    int32 ShotsFired;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
    int32 ShotsHit;
};

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABlackRemasteredWeapon();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void Initialize(ABlackRemasteredCharacter* OwnerCharacter, const FWeaponData& WeaponData);
    
    // Fire
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void Fire();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void StopFire();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void FireProjectile();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void FireHitscan();
    
    // Reload
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void Reload();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void TacticalReload();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void CancelReload();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void CompleteReload();
    
    // Melee
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void Melee();
    
    // Ammo
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void AddAmmo(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void AddReserveAmmo(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void SetAmmo(int32 Magazine, int32 Reserve);
    
    // Fire mode
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void ToggleFireMode();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void SetFireMode(EWeaponFireMode NewFireMode);
    
    // Attachments
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void AddAttachment(EWeaponAttachment Attachment);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void RemoveAttachment(EWeaponAttachment Attachment);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    virtual bool HasAttachment(EWeaponAttachment Attachment) const;
    
    // Overheat
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void Overheat();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void CoolDown(float DeltaTime);
    
    // Jam
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void Jam();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void ClearJam();
    
    // State
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    virtual void SetWeaponState(EWeaponState NewState);
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    EWeaponType GetWeaponType() const { return WeaponData.WeaponType; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    EWeaponState GetWeaponState() const { return CurrentState; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    EWeaponFireMode GetFireMode() const { return CurrentFireMode; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    int32 GetCurrentAmmo() const { return WeaponData.CurrentAmmo; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    int32 GetReserveAmmo() const { return WeaponData.ReserveAmmo; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    int32 GetMagazineCapacity() const { return WeaponData.MagazineCapacity; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    int32 GetMaxAmmo() const { return WeaponData.MaxAmmo; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    float GetBaseDamage() const { return WeaponData.BaseDamage; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    float GetFireRate() const { return WeaponData.FireRate; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    float GetCurrentHeat() const { return CurrentHeat; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool IsOverheated() const { return CurrentState == WS_Overheated; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool IsJammed() const { return CurrentState == WS_Jammed; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool IsReloading() const { return CurrentState == WS_Reloading; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool IsFiring() const { return CurrentState == WS_Firing; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool CanFire() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool CanReload() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool CanMelee() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool HasAmmo() const { return WeaponData.CurrentAmmo > 0; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool HasReserveAmmo() const { return WeaponData.ReserveAmmo > 0; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool IsFull() const { return WeaponData.CurrentAmmo >= WeaponData.MagazineCapacity; }
    
    // Owner
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    ABlackRemasteredCharacter* GetOwnerCharacter() const { return OwnerCharacter; }
    
    // Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Weapon")
    FWeaponData WeaponData;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFire);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnFire OnFire;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReload);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnReload OnReload;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadComplete);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnReloadComplete OnReloadComplete;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChanged);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnAmmoChanged OnAmmoChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateChanged);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnStateChanged OnStateChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverheat);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnOverheat OnOverheat;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJam);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnJam OnJam;
    
protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    USkeletalMeshComponent* WeaponMesh;
    
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    EWeaponState CurrentState;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    EWeaponFireMode CurrentFireMode;
    
    // Timers
    UPROPERTY()
    FTimerHandle FireTimerHandle;
    
    UPROPERTY()
    FTimerHandle ReloadTimerHandle;
    
    UPROPERTY()
    FTimerHandle OverheatTimerHandle;
    
    UPROPERTY()
    FTimerHandle JamTimerHandle;
    
    UPROPERTY()
    FTimerHandle BurstFireTimerHandle;
    
    // Burst fire
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    int32 BurstShotsFired;
    
    // Heat
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    float CurrentHeat;
    
    // Helper functions
    void OnFireTimer();
    void OnReloadTimer();
    void OnOverheatTimer();
    void OnJamTimer();
    void OnBurstFireTimer();
    
    void PlayFireEffects();
    void PlayReloadEffects();
    void PlayMeleeEffects();
    void PlayJamEffects();
    void PlayOverheatEffects();
    
    void SpawnProjectile();
    void PerformHitscan();
    
    FHitResult PerformWeaponTrace(FVector& Start, FVector& End);
    
    void ApplyRecoil();
    void ApplySpread(FVector& Direction);
    
    void ConsumeAmmo();
    
    void CheckForJam();
    void CheckForOverheat();
    
    // Fire rate tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    float LastFireTime;
    
    // Recoil tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    float CurrentVerticalRecoil;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    float CurrentHorizontalRecoil;
    
    // Spread tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    float CurrentSpread;
    
    // First shot accuracy
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    bool bFirstShotAccuracy;
};
