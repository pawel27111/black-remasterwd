#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackRemastered.h"
#include "BlackRemasteredInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAdded, ABlackRemasteredWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponRemoved, ABlackRemasteredWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquipped, ABlackRemasteredWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, TotalAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrenadeChanged, int32, GrenadeCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

class ABlackRemasteredWeapon;
class ABlackRemasteredCharacter;
class UBlackRemasteredWeaponComponent;
class ABlackRemasteredGrenade;

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    ABlackRemasteredWeapon* Weapon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 SlotIndex;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    bool bIsEquipped;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    bool bIsLocked;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    EAmmoType AmmoType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 Count;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 MaxCount;
};

USTRUCT(BlueprintType)
struct FGrenadeData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
    TSubclassOf<ABlackRemasteredGrenade> GrenadeClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
    int32 Count;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
    int32 MaxCount;
};

USTRUCT(BlueprintType)
struct FEquipmentData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FName EquipmentName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    bool bIsEquipped;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    bool bIsUnlocked;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKREMASTERED_API UBlackRemasteredInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBlackRemasteredInventoryComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void Initialize(ABlackRemasteredCharacter* OwnerCharacter);
    
    // Weapon management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool AddWeapon(ABlackRemasteredWeapon* Weapon);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool RemoveWeapon(ABlackRemasteredWeapon* Weapon);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool RemoveWeaponByIndex(int32 Index);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool EquipWeapon(ABlackRemasteredWeapon* Weapon);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool EquipWeaponByIndex(int32 Index);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool EquipNextWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool EquipPreviousWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void EquipDefaultWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void DropWeapon(ABlackRemasteredWeapon* Weapon);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void DropCurrentWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void SwapWeapons(int32 Index1, int32 Index2);
    
    // Ammo management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void AddAmmo(EAmmoType AmmoType, int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void RemoveAmmo(EAmmoType AmmoType, int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    int32 GetAmmoCount(EAmmoType AmmoType) const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    int32 GetMaxAmmo(EAmmoType AmmoType) const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool HasAmmo(EAmmoType AmmoType, int32 Amount = 1) const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void SetAmmo(EAmmoType AmmoType, int32 Amount);
    
    // Grenade management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void AddGrenade(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass, int32 Amount = 1);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void RemoveGrenade(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass, int32 Amount = 1);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    int32 GetGrenadeCount(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass) const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool HasGrenade(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass, int32 Amount = 1) const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void ThrowGrenade();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void ThrowGrenadeByClass(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass);
    
    // Equipment management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void AddEquipment(FName EquipmentName);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void RemoveEquipment(FName EquipmentName);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool HasEquipment(FName EquipmentName) const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void EquipEquipment(FName EquipmentName);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    void UnequipEquipment(FName EquipmentName);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Inventory")
    bool IsEquipmentEquipped(FName EquipmentName) const;
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    TArray<ABlackRemasteredWeapon*> GetAllWeapons() const { return Weapons; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    TArray<FInventorySlot> GetAllSlots() const { return Slots; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    ABlackRemasteredWeapon* GetCurrentWeapon() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    ABlackRemasteredWeapon* GetWeaponByIndex(int32 Index) const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    int32 GetWeaponCount() const { return Weapons.Num(); }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    int32 GetCurrentWeaponIndex() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    int32 GetMaxWeapons() const { return MaxWeapons; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    bool HasWeapon(ABlackRemasteredWeapon* Weapon) const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    bool HasWeaponOfType(EWeaponType WeaponType) const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    int32 GetWeaponIndex(ABlackRemasteredWeapon* Weapon) const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    int32 GetTotalAmmo(EAmmoType AmmoType) const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    int32 GetTotalAmmo() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    TArray<FAmmoData> GetAllAmmo() const { return AmmoData; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    TArray<FGrenadeData> GetAllGrenades() const { return GrenadeData; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    TArray<FEquipmentData> GetAllEquipment() const { return EquipmentData; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    bool IsFull() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Inventory")
    int32 GetSlotCount() const { return Slots.Num(); }
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Inventory")
    FOnWeaponAdded OnWeaponAdded;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Inventory")
    FOnWeaponRemoved OnWeaponRemoved;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Inventory")
    FOnWeaponEquipped OnWeaponEquipped;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Inventory")
    FOnAmmoChanged OnAmmoChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Inventory")
    FOnGrenadeChanged OnGrenadeChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Inventory")
    FOnInventoryChanged OnInventoryChanged;
    
protected:
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // Weapon component reference
    UPROPERTY()
    UBlackRemasteredWeaponComponent* WeaponComponent;
    
    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Inventory")
    int32 MaxWeapons;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Inventory")
    int32 MaxAmmoPerType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Inventory")
    int32 MaxGrenades;
    
    // Weapons
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Inventory")
    TArray<ABlackRemasteredWeapon*> Weapons;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Inventory")
    TArray<FInventorySlot> Slots;
    
    // Ammo
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Inventory")
    TArray<FAmmoData> AmmoData;
    
    // Grenades
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Inventory")
    TArray<FGrenadeData> GrenadeData;
    
    // Equipment
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Inventory")
    TArray<FEquipmentData> EquipmentData;
    
    // Helper functions
    void UpdateWeaponList();
    void UpdateAmmoData();
    void UpdateGrenadeData();
    
    int32 FindWeaponIndex(ABlackRemasteredWeapon* Weapon) const;
    int32 FindAmmoIndex(EAmmoType AmmoType) const;
    int32 FindGrenadeIndex(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass) const;
    int32 FindEquipmentIndex(FName EquipmentName) const;
    
    void BroadcastInventoryChanged();
};
