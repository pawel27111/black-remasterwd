#include "BlackRemasteredInventoryComponent.h"
#include "BlackRemasteredCharacter.h"
#include "Weapons/BlackRemasteredWeapon.h"
#include "Weapons/BlackRemasteredWeaponComponent.h"
#include "Weapons/BlackRemasteredGrenade.h"
#include "Core/BlackRemasteredGameState.h"

UBlackRemasteredInventoryComponent::UBlackRemasteredInventoryComponent()
    : Super()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Initialize state
    OwnerCharacter = nullptr;
    WeaponComponent = nullptr;
    
    // Settings
    MaxWeapons = 10;
    MaxAmmoPerType = 500;
    MaxGrenades = 10;
}

void UBlackRemasteredInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Get owner character
    if (AActor* Owner = GetOwner())
    {
        OwnerCharacter = Cast<ABlackRemasteredCharacter>(Owner);
    }
    
    // Get weapon component
    if (OwnerCharacter)
    {
        WeaponComponent = OwnerCharacter->GetWeaponComponent();
    }
    
    // Initialize default ammo
    for (int32 i = 0; i < static_cast<int32>(EAmmoType::AT_MAX); i++)
    {
        FAmmoData NewAmmoData;
        NewAmmoData.AmmoType = static_cast<EAmmoType>(i);
        NewAmmoData.Count = 0;
        NewAmmoData.MaxCount = MaxAmmoPerType;
        AmmoData.Add(NewAmmoData);
    }
}

void UBlackRemasteredInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBlackRemasteredInventoryComponent::Initialize(ABlackRemasteredCharacter* Owner)
{
    OwnerCharacter = Owner;
    
    // Get weapon component
    if (OwnerCharacter)
    {
        WeaponComponent = OwnerCharacter->GetWeaponComponent();
    }
    
    // Initialize default ammo
    for (int32 i = 0; i < static_cast<int32>(EAmmoType::AT_MAX); i++)
    {
        FAmmoData NewAmmoData;
        NewAmmoData.AmmoType = static_cast<EAmmoType>(i);
        NewAmmoData.Count = 0;
        NewAmmoData.MaxCount = MaxAmmoPerType;
        AmmoData.Add(NewAmmoData);
    }
}

bool UBlackRemasteredInventoryComponent::AddWeapon(ABlackRemasteredWeapon* Weapon)
{
    if (!Weapon)
    {
        return false;
    }
    
    // Check if already have this weapon
    if (HasWeapon(Weapon))
    {
        return false;
    }
    
    // Check if inventory is full
    if (IsFull())
    {
        return false;
    }
    
    // Add weapon to inventory
    Weapons.Add(Weapon);
    
    // Create slot for weapon
    FInventorySlot NewSlot;
    NewSlot.Weapon = Weapon;
    NewSlot.SlotIndex = Slots.Num();
    NewSlot.bIsEquipped = false;
    NewSlot.bIsLocked = false;
    Slots.Add(NewSlot);
    
    // Update weapon list in weapon component
    if (WeaponComponent)
    {
        WeaponComponent->UpdateWeaponList();
    }
    
    // Broadcast weapon added event
    OnWeaponAdded.Broadcast(Weapon);
    BroadcastInventoryChanged();
    
    return true;
}

bool UBlackRemasteredInventoryComponent::RemoveWeapon(ABlackRemasteredWeapon* Weapon)
{
    if (!Weapon)
    {
        return false;
    }
    
    int32 WeaponIndex = FindWeaponIndex(Weapon);
    if (WeaponIndex == INDEX_NONE)
    {
        return false;
    }
    
    return RemoveWeaponByIndex(WeaponIndex);
}

bool UBlackRemasteredInventoryComponent::RemoveWeaponByIndex(int32 Index)
{
    if (Index < 0 || Index >= Weapons.Num())
    {
        return false;
    }
    
    ABlackRemasteredWeapon* Weapon = Weapons[Index];
    
    // Check if weapon is equipped
    if (WeaponComponent && WeaponComponent->GetCurrentWeapon() == Weapon)
    {
        // Unequip weapon first
        WeaponComponent->UnequipWeapon();
    }
    
    // Remove weapon from inventory
    Weapons.RemoveAt(Index);
    
    // Remove slot
    Slots.RemoveAt(Index);
    
    // Update slot indices
    for (int32 i = Index; i < Slots.Num(); i++)
    {
        Slots[i].SlotIndex = i;
    }
    
    // Update weapon list in weapon component
    if (WeaponComponent)
    {
        WeaponComponent->UpdateWeaponList();
    }
    
    // Broadcast weapon removed event
    OnWeaponRemoved.Broadcast(Weapon);
    BroadcastInventoryChanged();
    
    return true;
}

bool UBlackRemasteredInventoryComponent::EquipWeapon(ABlackRemasteredWeapon* Weapon)
{
    if (!Weapon)
    {
        return false;
    }
    
    // Check if we have this weapon
    if (!HasWeapon(Weapon))
    {
        return false;
    }
    
    // Equip through weapon component
    if (WeaponComponent)
    {
        WeaponComponent->EquipWeapon(Weapon);
        
        // Update slot state
        int32 SlotIndex = FindWeaponIndex(Weapon);
        if (SlotIndex != INDEX_NONE && SlotIndex < Slots.Num())
        {
            Slots[SlotIndex].bIsEquipped = true;
        }
        
        // Broadcast weapon equipped event
        OnWeaponEquipped.Broadcast(Weapon);
        BroadcastInventoryChanged();
        
        return true;
    }
    
    return false;
}

bool UBlackRemasteredInventoryComponent::EquipWeaponByIndex(int32 Index)
{
    if (Index < 0 || Index >= Weapons.Num())
    {
        return false;
    }
    
    return EquipWeapon(Weapons[Index]);
}

bool UBlackRemasteredInventoryComponent::EquipNextWeapon()
{
    if (WeaponComponent)
    {
        WeaponComponent->EquipNextWeapon();
        
        // Update slot states
        int32 CurrentIndex = GetCurrentWeaponIndex();
        for (FInventorySlot& Slot : Slots)
        {
            Slot.bIsEquipped = (Slot.SlotIndex == CurrentIndex);
        }
        
        BroadcastInventoryChanged();
        return true;
    }
    
    return false;
}

bool UBlackRemasteredInventoryComponent::EquipPreviousWeapon()
{
    if (WeaponComponent)
    {
        WeaponComponent->EquipPreviousWeapon();
        
        // Update slot states
        int32 CurrentIndex = GetCurrentWeaponIndex();
        for (FInventorySlot& Slot : Slots)
        {
            Slot.bIsEquipped = (Slot.SlotIndex == CurrentIndex);
        }
        
        BroadcastInventoryChanged();
        return true;
    }
    
    return false;
}

void UBlackRemasteredInventoryComponent::EquipDefaultWeapon()
{
    // Try to equip first available weapon
    for (ABlackRemasteredWeapon* Weapon : Weapons)
    {
        if (EquipWeapon(Weapon))
        {
            return;
        }
    }
    
    // If no weapons, try to add a default weapon
    // TODO: Spawn default weapon
}

void UBlackRemasteredInventoryComponent::DropWeapon(ABlackRemasteredWeapon* Weapon)
{
    if (!Weapon)
    {
        return;
    }
    
    // Check if we have this weapon
    if (!HasWeapon(Weapon))
    {
        return;
    }
    
    // Check if weapon is equipped
    if (WeaponComponent && WeaponComponent->GetCurrentWeapon() == Weapon)
    {
        // Unequip first
        WeaponComponent->UnequipWeapon();
    }
    
    // Remove from inventory
    RemoveWeapon(Weapon);
    
    // TODO: Spawn weapon in world
}

void UBlackRemasteredInventoryComponent::DropCurrentWeapon()
{
    if (WeaponComponent && WeaponComponent->GetCurrentWeapon())
    {
        DropWeapon(WeaponComponent->GetCurrentWeapon());
    }
}

void UBlackRemasteredInventoryComponent::SwapWeapons(int32 Index1, int32 Index2)
{
    if (Index1 < 0 || Index1 >= Weapons.Num() || Index2 < 0 || Index2 >= Weapons.Num())
    {
        return;
    }
    
    if (Index1 == Index2)
    {
        return;
    }
    
    // Swap weapons in array
    Swap(Weapons[Index1], Weapons[Index2]);
    
    // Swap slots
    Swap(Slots[Index1], Slots[Index2]);
    
    // Update slot indices
    Slots[Index1].SlotIndex = Index1;
    Slots[Index2].SlotIndex = Index2;
    
    // Update weapon list in weapon component
    if (WeaponComponent)
    {
        WeaponComponent->UpdateWeaponList();
    }
    
    BroadcastInventoryChanged();
}

void UBlackRemasteredInventoryComponent::AddAmmo(EAmmoType AmmoType, int32 Amount)
{
    int32 AmmoIndex = FindAmmoIndex(AmmoType);
    if (AmmoIndex == INDEX_NONE)
    {
        return;
    }
    
    // Add ammo
    AmmoData[AmmoIndex].Count = FMath::Min(AmmoData[AmmoIndex].Count + Amount, AmmoData[AmmoIndex].MaxCount);
    
    // Broadcast ammo changed event
    OnAmmoChanged.Broadcast(GetTotalAmmo(AmmoType));
    BroadcastInventoryChanged();
}

void UBlackRemasteredInventoryComponent::RemoveAmmo(EAmmoType AmmoType, int32 Amount)
{
    int32 AmmoIndex = FindAmmoIndex(AmmoType);
    if (AmmoIndex == INDEX_NONE)
    {
        return;
    }
    
    // Remove ammo
    AmmoData[AmmoIndex].Count = FMath::Max(AmmoData[AmmoIndex].Count - Amount, 0);
    
    // Broadcast ammo changed event
    OnAmmoChanged.Broadcast(GetTotalAmmo(AmmoType));
    BroadcastInventoryChanged();
}

int32 UBlackRemasteredInventoryComponent::GetAmmoCount(EAmmoType AmmoType) const
{
    int32 AmmoIndex = FindAmmoIndex(AmmoType);
    if (AmmoIndex == INDEX_NONE)
    {
        return 0;
    }
    
    return AmmoData[AmmoIndex].Count;
}

int32 UBlackRemasteredInventoryComponent::GetMaxAmmo(EAmmoType AmmoType) const
{
    int32 AmmoIndex = FindAmmoIndex(AmmoType);
    if (AmmoIndex == INDEX_NONE)
    {
        return 0;
    }
    
    return AmmoData[AmmoIndex].MaxCount;
}

bool UBlackRemasteredInventoryComponent::HasAmmo(EAmmoType AmmoType, int32 Amount) const
{
    return GetAmmoCount(AmmoType) >= Amount;
}

void UBlackRemasteredInventoryComponent::SetAmmo(EAmmoType AmmoType, int32 Amount)
{
    int32 AmmoIndex = FindAmmoIndex(AmmoType);
    if (AmmoIndex == INDEX_NONE)
    {
        return;
    }
    
    // Set ammo
    AmmoData[AmmoIndex].Count = FMath::Clamp(Amount, 0, AmmoData[AmmoIndex].MaxCount);
    
    // Broadcast ammo changed event
    OnAmmoChanged.Broadcast(GetTotalAmmo(AmmoType));
    BroadcastInventoryChanged();
}

void UBlackRemasteredInventoryComponent::AddGrenade(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass, int32 Amount)
{
    int32 GrenadeIndex = FindGrenadeIndex(GrenadeClass);
    
    if (GrenadeIndex == INDEX_NONE)
    {
        // Add new grenade type
        FGrenadeData NewGrenadeData;
        NewGrenadeData.GrenadeClass = GrenadeClass;
        NewGrenadeData.Count = Amount;
        NewGrenadeData.MaxCount = MaxGrenades;
        GrenadeData.Add(NewGrenadeData);
        GrenadeIndex = GrenadeData.Num() - 1;
    }
    else
    {
        // Add to existing grenade type
        GrenadeData[GrenadeIndex].Count = FMath::Min(GrenadeData[GrenadeIndex].Count + Amount, GrenadeData[GrenadeIndex].MaxCount);
    }
    
    // Broadcast grenade changed event
    OnGrenadeChanged.Broadcast(GetGrenadeCount(GrenadeClass));
    BroadcastInventoryChanged();
}

void UBlackRemasteredInventoryComponent::RemoveGrenade(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass, int32 Amount)
{
    int32 GrenadeIndex = FindGrenadeIndex(GrenadeClass);
    if (GrenadeIndex == INDEX_NONE)
    {
        return;
    }
    
    // Remove grenade
    GrenadeData[GrenadeIndex].Count = FMath::Max(GrenadeData[GrenadeIndex].Count - Amount, 0);
    
    // Broadcast grenade changed event
    OnGrenadeChanged.Broadcast(GetGrenadeCount(GrenadeClass));
    BroadcastInventoryChanged();
}

int32 UBlackRemasteredInventoryComponent::GetGrenadeCount(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass) const
{
    int32 GrenadeIndex = FindGrenadeIndex(GrenadeClass);
    if (GrenadeIndex == INDEX_NONE)
    {
        return 0;
    }
    
    return GrenadeData[GrenadeIndex].Count;
}

bool UBlackRemasteredInventoryComponent::HasGrenade(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass, int32 Amount) const
{
    return GetGrenadeCount(GrenadeClass) >= Amount;
}

void UBlackRemasteredInventoryComponent::ThrowGrenade()
{
    if (!WeaponComponent)
    {
        return;
    }
    
    // Get current weapon to determine grenade type
    ABlackRemasteredWeapon* CurrentWeapon = WeaponComponent->GetCurrentWeapon();
    if (!CurrentWeapon)
    {
        return;
    }
    
    // TODO: Get grenade class from weapon or inventory
    // For now, use first available grenade
    for (FGrenadeData& Grenade : GrenadeData)
    {
        if (Grenade.Count > 0 && Grenade.GrenadeClass)
        {
            ThrowGrenadeByClass(Grenade.GrenadeClass);
            return;
        }
    }
}

void UBlackRemasteredInventoryComponent::ThrowGrenadeByClass(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass)
{
    if (!GrenadeClass)
    {
        return;
    }
    
    // Check if we have this grenade
    if (!HasGrenade(GrenadeClass))
    {
        return;
    }
    
    // Remove grenade from inventory
    RemoveGrenade(GrenadeClass, 1);
    
    // Spawn and throw grenade
    if (OwnerCharacter)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.Owner = OwnerCharacter;
        SpawnParams.Instigator = OwnerCharacter->GetInstigator();
        
        ABlackRemasteredGrenade* Grenade = GetWorld()->SpawnActor<ABlackRemasteredGrenade>(
            GrenadeClass,
            OwnerCharacter->GetActorLocation() + FVector(0, 0, 50),
            OwnerCharacter->GetActorRotation(),
            SpawnParams
        );
        
        if (Grenade)
        {
            Grenade->Throw(OwnerCharacter);
        }
    }
    
    // Update game state stats
    if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
    {
        GameState->AddGrenade();
    }
}

void UBlackRemasteredInventoryComponent::AddEquipment(FName EquipmentName)
{
    int32 EquipmentIndex = FindEquipmentIndex(EquipmentName);
    
    if (EquipmentIndex == INDEX_NONE)
    {
        // Add new equipment
        FEquipmentData NewEquipmentData;
        NewEquipmentData.EquipmentName = EquipmentName;
        NewEquipmentData.bIsEquipped = false;
        NewEquipmentData.bIsUnlocked = true;
        EquipmentData.Add(NewEquipmentData);
    }
    else
    {
        // Equipment already exists, just mark as unlocked
        EquipmentData[EquipmentIndex].bIsUnlocked = true;
    }
    
    BroadcastInventoryChanged();
}

void UBlackRemasteredInventoryComponent::RemoveEquipment(FName EquipmentName)
{
    int32 EquipmentIndex = FindEquipmentIndex(EquipmentName);
    if (EquipmentIndex == INDEX_NONE)
    {
        return;
    }
    
    // Remove equipment
    EquipmentData.RemoveAt(EquipmentIndex);
    
    BroadcastInventoryChanged();
}

bool UBlackRemasteredInventoryComponent::HasEquipment(FName EquipmentName) const
{
    return FindEquipmentIndex(EquipmentName) != INDEX_NONE;
}

void UBlackRemasteredInventoryComponent::EquipEquipment(FName EquipmentName)
{
    int32 EquipmentIndex = FindEquipmentIndex(EquipmentName);
    if (EquipmentIndex == INDEX_NONE)
    {
        return;
    }
    
    // Check if unlocked
    if (!EquipmentData[EquipmentIndex].bIsUnlocked)
    {
        return;
    }
    
    // Equip equipment
    EquipmentData[EquipmentIndex].bIsEquipped = true;
    
    // TODO: Apply equipment effects
    
    BroadcastInventoryChanged();
}

void UBlackRemasteredInventoryComponent::UnequipEquipment(FName EquipmentName)
{
    int32 EquipmentIndex = FindEquipmentIndex(EquipmentName);
    if (EquipmentIndex == INDEX_NONE)
    {
        return;
    }
    
    // Unequip equipment
    EquipmentData[EquipmentIndex].bIsEquipped = false;
    
    // TODO: Remove equipment effects
    
    BroadcastInventoryChanged();
}

bool UBlackRemasteredInventoryComponent::IsEquipmentEquipped(FName EquipmentName) const
{
    int32 EquipmentIndex = FindEquipmentIndex(EquipmentName);
    if (EquipmentIndex == INDEX_NONE)
    {
        return false;
    }
    
    return EquipmentData[EquipmentIndex].bIsEquipped;
}

ABlackRemasteredWeapon* UBlackRemasteredInventoryComponent::GetCurrentWeapon() const
{
    if (WeaponComponent)
    {
        return WeaponComponent->GetCurrentWeapon();
    }
    
    return nullptr;
}

ABlackRemasteredWeapon* UBlackRemasteredInventoryComponent::GetWeaponByIndex(int32 Index) const
{
    if (Index < 0 || Index >= Weapons.Num())
    {
        return nullptr;
    }
    
    return Weapons[Index];
}

int32 UBlackRemasteredInventoryComponent::GetCurrentWeaponIndex() const
{
    if (WeaponComponent)
    {
        return WeaponComponent->GetCurrentWeaponIndex();
    }
    
    return -1;
}

bool UBlackRemasteredInventoryComponent::HasWeapon(ABlackRemasteredWeapon* Weapon) const
{
    return Weapons.Contains(Weapon);
}

bool UBlackRemasteredInventoryComponent::HasWeaponOfType(EWeaponType WeaponType) const
{
    for (ABlackRemasteredWeapon* Weapon : Weapons)
    {
        if (Weapon && Weapon->GetWeaponType() == WeaponType)
        {
            return true;
        }
    }
    
    return false;
}

int32 UBlackRemasteredInventoryComponent::GetWeaponIndex(ABlackRemasteredWeapon* Weapon) const
{
    return Weapons.Find(Weapon);
}

int32 UBlackRemasteredInventoryComponent::GetTotalAmmo(EAmmoType AmmoType) const
{
    int32 Total = GetAmmoCount(AmmoType);
    
    // Add ammo from all weapons
    for (ABlackRemasteredWeapon* Weapon : Weapons)
    {
        if (Weapon && Weapon->GetAmmoType() == AmmoType)
        {
            Total += Weapon->GetCurrentAmmo();
            Total += Weapon->GetReserveAmmo();
        }
    }
    
    return Total;
}

int32 UBlackRemasteredInventoryComponent::GetTotalAmmo() const
{
    int32 Total = 0;
    
    for (FAmmoData Ammo : AmmoData)
    {
        Total += Ammo.Count;
    }
    
    // Add ammo from all weapons
    for (ABlackRemasteredWeapon* Weapon : Weapons)
    {
        if (Weapon)
        {
            Total += Weapon->GetCurrentAmmo();
            Total += Weapon->GetReserveAmmo();
        }
    }
    
    return Total;
}

bool UBlackRemasteredInventoryComponent::IsFull() const
{
    return Weapons.Num() >= MaxWeapons;
}

void UBlackRemasteredInventoryComponent::UpdateWeaponList()
{
    // This is called when weapons are added/removed
    // Update weapon component's weapon list
    if (WeaponComponent)
    {
        WeaponComponent->UpdateWeaponList();
    }
}

void UBlackRemasteredInventoryComponent::UpdateAmmoData()
{
    // Update ammo data from weapons
    // This is called when weapons are added/removed
}

void UBlackRemasteredInventoryComponent::UpdateGrenadeData()
{
    // Update grenade data
    // This is called when grenades are added/removed
}

int32 UBlackRemasteredInventoryComponent::FindWeaponIndex(ABlackRemasteredWeapon* Weapon) const
{
    return Weapons.Find(Weapon);
}

int32 UBlackRemasteredInventoryComponent::FindAmmoIndex(EAmmoType AmmoType) const
{
    for (int32 i = 0; i < AmmoData.Num(); i++)
    {
        if (AmmoData[i].AmmoType == AmmoType)
        {
            return i;
        }
    }
    
    return INDEX_NONE;
}

int32 UBlackRemasteredInventoryComponent::FindGrenadeIndex(TSubclassOf<ABlackRemasteredGrenade> GrenadeClass) const
{
    for (int32 i = 0; i < GrenadeData.Num(); i++)
    {
        if (GrenadeData[i].GrenadeClass == GrenadeClass)
        {
            return i;
        }
    }
    
    return INDEX_NONE;
}

int32 UBlackRemasteredInventoryComponent::FindEquipmentIndex(FName EquipmentName) const
{
    for (int32 i = 0; i < EquipmentData.Num(); i++)
    {
        if (EquipmentData[i].EquipmentName == EquipmentName)
        {
            return i;
        }
    }
    
    return INDEX_NONE;
}

void UBlackRemasteredInventoryComponent::BroadcastInventoryChanged()
{
    OnInventoryChanged.Broadcast();
}
