#include "BlackRemasteredWeaponComponent.h"
#include "BlackRemasteredWeapon.h"
#include "BlackRemasteredCharacter.h"
#include "Gameplay/BlackRemasteredInventoryComponent.h"
#include "Core/BlackRemasteredGameState.h"

UBlackRemasteredWeaponComponent::UBlackRemasteredWeaponComponent()
    : Super()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Initialize state
    OwnerCharacter = nullptr;
    InventoryComponent = nullptr;
    CurrentWeapon = nullptr;
    CurrentWeaponIndex = -1;
    bIsAiming = false;
    bIsFiring = false;
    
    // Settings
    AimFOV = 60.0f;
    NormalFOV = 90.0f;
    AimTransitionSpeed = 10.0f;
}

void UBlackRemasteredWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Get owner character
    if (AActor* Owner = GetOwner())
    {
        OwnerCharacter = Cast<ABlackRemasteredCharacter>(Owner);
    }
    
    // Get inventory component
    if (OwnerCharacter)
    {
        InventoryComponent = OwnerCharacter->GetInventoryComponent();
    }
    
    // Update weapon list
    UpdateWeaponList();
}

void UBlackRemasteredWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Update aim state
    UpdateAimState();
}

void UBlackRemasteredWeaponComponent::Initialize(ABlackRemasteredCharacter* Owner)
{
    OwnerCharacter = Owner;
    
    // Get inventory component
    if (OwnerCharacter)
    {
        InventoryComponent = OwnerCharacter->GetInventoryComponent();
    }
    
    // Update weapon list
    UpdateWeaponList();
    
    // Equip default weapon
    if (InventoryComponent)
    {
        InventoryComponent->EquipDefaultWeapon();
    }
}

void UBlackRemasteredWeaponComponent::Cleanup()
{
    // Unequip current weapon
    UnequipWeapon();
    
    // Clear weapon list
    Weapons.Empty();
    CurrentWeapon = nullptr;
    CurrentWeaponIndex = -1;
}

void UBlackRemasteredWeaponComponent::EquipWeapon(ABlackRemasteredWeapon* Weapon)
{
    if (!Weapon)
    {
        return;
    }
    
    // Check if already equipped
    if (CurrentWeapon == Weapon)
    {
        return;
    }
    
    // Get current weapon for switching event
    ABlackRemasteredWeapon* OldWeapon = CurrentWeapon;
    
    // Unequip current weapon
    if (CurrentWeapon)
    {
        // Hide current weapon
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon->SetActorTickEnabled(false);
        
        // Broadcast unequipped event
        OnWeaponUnequipped.Broadcast(CurrentWeapon);
    }
    
    // Equip new weapon
    CurrentWeapon = Weapon;
    CurrentWeaponIndex = Weapons.Find(Weapon);
    
    if (CurrentWeaponIndex == INDEX_NONE)
    {
        // Weapon not in list, add it
        Weapons.Add(Weapon);
        CurrentWeaponIndex = Weapons.Num() - 1;
    }
    
    // Show new weapon
    CurrentWeapon->SetActorHiddenInGame(false);
    CurrentWeapon->SetActorEnableCollision(true);
    CurrentWeapon->SetActorTickEnabled(true);
    
    // Initialize weapon
    if (OwnerCharacter)
    {
        // TODO: Get weapon data from inventory
        FWeaponData DefaultData;
        CurrentWeapon->Initialize(OwnerCharacter, DefaultData);
    }
    
    // Broadcast equipped event
    OnWeaponEquipped.Broadcast(CurrentWeapon);
    
    // Broadcast switched event
    if (OldWeapon)
    {
        OnWeaponSwitched.Broadcast(OldWeapon, CurrentWeapon);
    }
}

void UBlackRemasteredWeaponComponent::UnequipWeapon()
{
    if (!CurrentWeapon)
    {
        return;
    }
    
    // Hide current weapon
    CurrentWeapon->SetActorHiddenInGame(true);
    CurrentWeapon->SetActorEnableCollision(false);
    CurrentWeapon->SetActorTickEnabled(false);
    
    // Broadcast unequipped event
    OnWeaponUnequipped.Broadcast(CurrentWeapon);
    
    // Clear current weapon
    ABlackRemasteredWeapon* OldWeapon = CurrentWeapon;
    CurrentWeapon = nullptr;
    CurrentWeaponIndex = -1;
    
    // Broadcast switched event
    OnWeaponSwitched.Broadcast(OldWeapon, nullptr);
}

void UBlackRemasteredWeaponComponent::EquipNextWeapon()
{
    if (Weapons.Num() <= 1)
    {
        return;
    }
    
    if (CurrentWeaponIndex < 0)
    {
        // No weapon equipped, equip first
        EquipWeaponByIndex(0);
        return;
    }
    
    // Get next weapon index
    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeaponByIndex(NextIndex);
}

void UBlackRemasteredWeaponComponent::EquipPreviousWeapon()
{
    if (Weapons.Num() <= 1)
    {
        return;
    }
    
    if (CurrentWeaponIndex < 0)
    {
        // No weapon equipped, equip first
        EquipWeaponByIndex(0);
        return;
    }
    
    // Get previous weapon index
    int32 PrevIndex = (CurrentWeaponIndex - 1 + Weapons.Num()) % Weapons.Num();
    EquipWeaponByIndex(PrevIndex);
}

void UBlackRemasteredWeaponComponent::EquipWeaponByIndex(int32 Index)
{
    if (Index < 0 || Index >= Weapons.Num())
    {
        return;
    }
    
    if (Weapons[Index])
    {
        EquipWeapon(Weapons[Index]);
    }
}

void UBlackRemasteredWeaponComponent::DropWeapon()
{
    if (!CurrentWeapon)
    {
        return;
    }
    
    // TODO: Implement weapon dropping
    // - Detach from character
    // - Spawn in world
    // - Remove from inventory
    
    UnequipWeapon();
}

void UBlackRemasteredWeaponComponent::Fire()
{
    if (!CurrentWeapon)
    {
        return;
    }
    
    bIsFiring = true;
    CurrentWeapon->Fire();
    
    // Broadcast fire event
    OnWeaponFired.Broadcast(CurrentWeapon);
}

void UBlackRemasteredWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
    {
        return;
    }
    
    bIsFiring = false;
    CurrentWeapon->StopFire();
}

void UBlackRemasteredWeaponComponent::Reload()
{
    if (!CurrentWeapon)
    {
        return;
    }
    
    CurrentWeapon->Reload();
    
    // Broadcast reload event
    OnWeaponReloaded.Broadcast(CurrentWeapon);
}

void UBlackRemasteredWeaponComponent::TacticalReload()
{
    if (!CurrentWeapon)
    {
        return;
    }
    
    CurrentWeapon->TacticalReload();
    
    // Broadcast reload event
    OnWeaponReloaded.Broadcast(CurrentWeapon);
}

void UBlackRemasteredWeaponComponent::Melee()
{
    if (!CurrentWeapon)
    {
        return;
    }
    
    CurrentWeapon->Melee();
}

void UBlackRemasteredWeaponComponent::StartAim()
{
    bIsAiming = true;
    
    // Update camera FOV
    if (OwnerCharacter && OwnerCharacter->GetFollowCamera())
    {
        OwnerCharacter->GetFollowCamera()->SetFieldOfView(AimFOV);
    }
    
    UpdateAimState();
}

void UBlackRemasteredWeaponComponent::StopAim()
{
    bIsAiming = false;
    
    // Restore camera FOV
    if (OwnerCharacter && OwnerCharacter->GetFollowCamera())
    {
        OwnerCharacter->GetFollowCamera()->SetFieldOfView(NormalFOV);
    }
    
    UpdateAimState();
}

void UBlackRemasteredWeaponComponent::ToggleFireMode()
{
    if (!CurrentWeapon)
    {
        return;
    }
    
    CurrentWeapon->ToggleFireMode();
}

ABlackRemasteredWeapon* UBlackRemasteredWeaponComponent::GetPreviousWeapon() const
{
    if (CurrentWeaponIndex <= 0)
    {
        return nullptr;
    }
    
    if (CurrentWeaponIndex - 1 < Weapons.Num())
    {
        return Weapons[CurrentWeaponIndex - 1];
    }
    
    return nullptr;
}

bool UBlackRemasteredWeaponComponent::HasWeapon(ABlackRemasteredWeapon* Weapon) const
{
    return Weapons.Contains(Weapon);
}

bool UBlackRemasteredWeaponComponent::CanFire() const
{
    if (!CurrentWeapon)
    {
        return false;
    }
    
    return CurrentWeapon->CanFire();
}

bool UBlackRemasteredWeaponComponent::CanReload() const
{
    if (!CurrentWeapon)
    {
        return false;
    }
    
    return CurrentWeapon->CanReload();
}

bool UBlackRemasteredWeaponComponent::CanMelee() const
{
    if (!CurrentWeapon)
    {
        return false;
    }
    
    return CurrentWeapon->CanMelee();
}

bool UBlackRemasteredWeaponComponent::IsFiring() const
{
    if (!CurrentWeapon)
    {
        return false;
    }
    
    return CurrentWeapon->IsFiring() || bIsFiring;
}

void UBlackRemasteredWeaponComponent::UpdateWeaponList()
{
    // Clear current list
    Weapons.Empty();
    CurrentWeaponIndex = -1;
    
    // Get weapons from inventory
    if (InventoryComponent)
    {
        Weapons = InventoryComponent->GetAllWeapons();
        
        // Find current weapon index
        if (CurrentWeapon)
        {
            CurrentWeaponIndex = Weapons.Find(CurrentWeapon);
        }
    }
}

void UBlackRemasteredWeaponComponent::UpdateAimState()
{
    if (!OwnerCharacter)
    {
        return;
    }
    
    // Update owner character aim state
    OwnerCharacter->SetAimState(bIsAiming);
}

void UBlackRemasteredWeaponComponent::OnWeaponFired(ABlackRemasteredWeapon* Weapon)
{
    // Update firing state
    bIsFiring = true;
    
    // Update game state stats
    if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
    {
        // Stats are updated in the weapon itself
    }
}

void UBlackRemasteredWeaponComponent::OnWeaponReloaded(ABlackRemasteredWeapon* Weapon)
{
    // Broadcast reload event
    OnWeaponReloaded.Broadcast(Weapon);
}
