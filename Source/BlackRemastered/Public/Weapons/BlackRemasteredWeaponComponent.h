#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackRemastered.h"
#include "BlackRemasteredWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquipped, ABlackRemasteredWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponUnequipped, ABlackRemasteredWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSwitched, ABlackRemasteredWeapon*, OldWeapon, ABlackRemasteredWeapon*, NewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFired, ABlackRemasteredWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloaded, ABlackRemasteredWeapon*, Weapon);

class ABlackRemasteredWeapon;
class ABlackRemasteredCharacter;
class UBlackRemasteredInventoryComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKREMASTERED_API UBlackRemasteredWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBlackRemasteredWeaponComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void Initialize(ABlackRemasteredCharacter* OwnerCharacter);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void Cleanup();
    
    // Weapon management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void EquipWeapon(ABlackRemasteredWeapon* Weapon);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void UnequipWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void EquipNextWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void EquipPreviousWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void EquipWeaponByIndex(int32 Index);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void DropWeapon();
    
    // Fire
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void Fire();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void StopFire();
    
    // Reload
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void Reload();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void TacticalReload();
    
    // Melee
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void Melee();
    
    // Aim
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void StartAim();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void StopAim();
    
    // Fire mode
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Weapon")
    void ToggleFireMode();
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    ABlackRemasteredWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    ABlackRemasteredWeapon* GetPreviousWeapon() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    TArray<ABlackRemasteredWeapon*> GetAllWeapons() const { return Weapons; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    int32 GetWeaponCount() const { return Weapons.Num(); }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool HasWeapon(ABlackRemasteredWeapon* Weapon) const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool CanFire() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool CanReload() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool CanMelee() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool IsAiming() const { return bIsAiming; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Weapon")
    bool IsFiring() const;
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnWeaponEquipped OnWeaponEquipped;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnWeaponUnequipped OnWeaponUnequipped;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnWeaponSwitched OnWeaponSwitched;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnWeaponFired OnWeaponFired;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Weapon")
    FOnWeaponReloaded OnWeaponReloaded;
    
protected:
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // Inventory reference
    UPROPERTY()
    UBlackRemasteredInventoryComponent* InventoryComponent;
    
    // Weapons
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    TArray<ABlackRemasteredWeapon*> Weapons;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    ABlackRemasteredWeapon* CurrentWeapon;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    int32 CurrentWeaponIndex;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    bool bIsAiming;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Weapon")
    bool bIsFiring;
    
    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Weapon")
    float AimFOV;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Weapon")
    float NormalFOV;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Weapon")
    float AimTransitionSpeed;
    
    // Helper functions
    void UpdateWeaponList();
    void UpdateAimState();
    
    void OnWeaponFired(ABlackRemasteredWeapon* Weapon);
    void OnWeaponReloaded(ABlackRemasteredWeapon* Weapon);
};
