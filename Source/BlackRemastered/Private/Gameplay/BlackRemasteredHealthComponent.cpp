#include "BlackRemasteredHealthComponent.h"
#include "BlackRemasteredCharacter.h"
#include "Core/BlackRemasteredGameMode.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

UBlackRemasteredHealthComponent::UBlackRemasteredHealthComponent()
    : Super()
{
    // Set this component to be initialized when the game starts
    PrimaryComponentTick.bCanEverTick = true;
    
    // Health settings
    MaxHealth = BlackRemasteredConstants::DEFAULT_MAX_HEALTH;
    CurrentHealth = MaxHealth;
    
    // Armor settings
    MaxArmor = BlackRemasteredConstants::DEFAULT_MAX_ARMOR;
    CurrentArmor = 0.0f;
    
    // Regeneration settings
    bRegenerateHealth = true;
    HealthRegenRate = BlackRemasteredConstants::DEFAULT_HEALTH_REGEN_RATE;
    HealthRegenDelay = BlackRemasteredConstants::DEFAULT_HEALTH_REGEN_DELAY;
    
    // Damage modifiers
    HeadshotDamageMultiplier = 2.0f;
    ArmorDamageReduction = 0.5f;
    
    // Invincibility
    bInvincible = false;
    InvincibilityDuration = 2.0f;
    
    // Initialize owner
    OwnerCharacter = nullptr;
}

void UBlackRemasteredHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Start health regeneration timer
    if (bRegenerateHealth)
    {
        StartHealthRegen();
    }
}

void UBlackRemasteredHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBlackRemasteredHealthComponent::Initialize(ABlackRemasteredCharacter* Owner)
{
    OwnerCharacter = Owner;
    
    // Apply difficulty settings if available
    if (ABlackRemasteredGameMode* GameMode = GetWorld()->GetAuthGameMode<ABlackRemasteredGameMode>())
    {
        // Max health can be affected by difficulty
        // For now, keep it simple
    }
}

void UBlackRemasteredHealthComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bInvincible)
    {
        return;
    }
    
    // Calculate actual damage
    float ActualDamage = CalculateActualDamage(Damage, DamageEvent);
    
    // Apply damage to armor first
    if (CurrentArmor > 0.0f)
    {
        float ArmorDamage = ActualDamage * (1.0f - ArmorDamageReduction);
        float ArmorAbsorbed = FMath::Min(CurrentArmor, ArmorDamage);
        
        CurrentArmor -= ArmorAbsorbed;
        ActualDamage -= ArmorAbsorbed / (1.0f - ArmorDamageReduction);
        
        // Clamp to minimum
        CurrentArmor = FMath::Max(CurrentArmor, 0.0f);
        
        // Broadcast armor change
        OnArmorChanged.Broadcast();
    }
    
    // Apply remaining damage to health
    if (ActualDamage > 0.0f)
    {
        CurrentHealth = FMath::Max(CurrentHealth - ActualDamage, 0.0f);
        
        // Broadcast health change
        OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, ActualDamage, DamageCauser);
        OnDamageTaken.Broadcast();
        
        // Stop health regeneration
        StopHealthRegen();
        
        // Start invincibility frames (optional)
        // bInvincible = true;
        // GetWorld()->GetTimerManager().SetTimer(
        //     InvincibilityTimerHandle,
        //     this,
        //     &UBlackRemasteredHealthComponent::OnInvincibilityTimer,
        //     InvincibilityDuration,
        //     false
        // );
        
        // Check for death
        if (CurrentHealth <= 0.0f)
        {
            CurrentHealth = 0.0f;
            OnHealthDepleted.Broadcast();
            
            // Notify owner
            if (OwnerCharacter)
            {
                OwnerCharacter->Die(EventInstigator);
            }
        }
        else
        {
            // Start delay before health regeneration
            GetWorld()->GetTimerManager().SetTimer(
                LastDamageTimerHandle,
                this,
                &UBlackRemasteredHealthComponent::OnLastDamageTimer,
                HealthRegenDelay,
                false
            );
        }
    }
}

void UBlackRemasteredHealthComponent::Heal(float Amount, AActor* Healer)
{
    if (CurrentHealth < MaxHealth)
    {
        CurrentHealth = FMath::Min(CurrentHealth + Amount, MaxHealth);
        
        // Broadcast health change
        OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, -Amount, Healer);
        OnHealthRestored.Broadcast();
    }
}

void UBlackRemasteredHealthComponent::ResetHealth()
{
    CurrentHealth = MaxHealth;
    CurrentArmor = 0.0f;
    
    // Broadcast health change
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, 0.0f, nullptr);
    OnArmorChanged.Broadcast();
}

void UBlackRemasteredHealthComponent::AddArmor(float Amount)
{
    CurrentArmor = FMath::Min(CurrentArmor + Amount, MaxArmor);
    
    // Broadcast armor change
    OnArmorChanged.Broadcast();
}

void UBlackRemasteredHealthComponent::RemoveArmor(float Amount)
{
    CurrentArmor = FMath::Max(CurrentArmor - Amount, 0.0f);
    
    // Broadcast armor change
    OnArmorChanged.Broadcast();
}

void UBlackRemasteredHealthComponent::SetArmor(float Amount)
{
    CurrentArmor = FMath::Clamp(Amount, 0.0f, MaxArmor);
    
    // Broadcast armor change
    OnArmorChanged.Broadcast();
}

float UBlackRemasteredHealthComponent::GetHealthPercentage() const
{
    if (MaxHealth <= 0.0f)
    {
        return 0.0f;
    }
    return CurrentHealth / MaxHealth * 100.0f;
}

float UBlackRemasteredHealthComponent::GetArmorPercentage() const
{
    if (MaxArmor <= 0.0f)
    {
        return 0.0f;
    }
    return CurrentArmor / MaxArmor * 100.0f;
}

void UBlackRemasteredHealthComponent::StartHealthRegen()
{
    // Clear existing timer
    GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
    
    // Start new timer
    GetWorld()->GetTimerManager().SetTimer(
        HealthRegenTimerHandle,
        this,
        &UBlackRemasteredHealthComponent::OnHealthRegenTimer,
        0.1f,
        true
    );
}

void UBlackRemasteredHealthComponent::StopHealthRegen()
{
    GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
}

void UBlackRemasteredHealthComponent::OnHealthRegenTimer()
{
    if (bRegenerateHealth && CurrentHealth < MaxHealth)
    {
        float HealAmount = HealthRegenRate * 0.1f;
        CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);
        
        // Broadcast health change
        OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, -HealAmount, nullptr);
        OnHealthRestored.Broadcast();
    }
}

void UBlackRemasteredHealthComponent::OnLastDamageTimer()
{
    // Start health regeneration after delay
    StartHealthRegen();
}

void UBlackRemasteredHealthComponent::OnInvincibilityTimer()
{
    bInvincible = false;
}

float UBlackRemasteredHealthComponent::CalculateActualDamage(float Damage, FDamageEvent const& DamageEvent)
{
    float ActualDamage = Damage;
    
    // Check for headshot
    if (IsHeadshot(DamageEvent))
    {
        ActualDamage *= HeadshotDamageMultiplier;
    }
    
    // Apply difficulty modifier
    if (ABlackRemasteredGameMode* GameMode = GetWorld()->GetAuthGameMode<ABlackRemasteredGameMode>())
    {
        ActualDamage *= GameMode->GetPlayerDamageMultiplier();
    }
    
    return ActualDamage;
}

bool UBlackRemasteredHealthComponent::IsHeadshot(FDamageEvent const& DamageEvent) const
{
    if (const UDamageType* DamageType = DamageEvent.DamageTypeClass ? DamageType->GetDefaultObject<UDamageType>() : nullptr)
    {
        // Check if this is a headshot
        // This would typically be determined by hit location
        // For now, we'll use a simple check
        
        // In a real implementation, you would check the hit bone name
        // or use a custom damage type with headshot flag
        return false;
    }
    
    return false;
}
