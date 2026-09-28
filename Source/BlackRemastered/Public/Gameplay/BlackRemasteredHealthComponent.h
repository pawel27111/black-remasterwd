#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackRemastered.h"
#include "BlackRemasteredHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth, float, DamageAmount, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthRestored);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArmorChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTaken);

class ABlackRemasteredCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKREMASTERED_API UBlackRemasteredHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBlackRemasteredHealthComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void Initialize(ABlackRemasteredCharacter* OwnerCharacter);
    
    // Health management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void Heal(float Amount, AActor* Healer);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void ResetHealth();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void AddArmor(float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void RemoveArmor(float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Health")
    void SetArmor(float Amount);
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetCurrentHealth() const { return CurrentHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetMaxHealth() const { return MaxHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetHealthPercentage() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetCurrentArmor() const { return CurrentArmor; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetMaxArmor() const { return MaxArmor; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    float GetArmorPercentage() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    bool IsAlive() const { return CurrentHealth > 0.0f; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    bool IsFullHealth() const { return CurrentHealth >= MaxHealth; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    bool HasArmor() const { return CurrentArmor > 0.0f; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Health")
    bool IsFullArmor() const { return CurrentArmor >= MaxArmor; }
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Health")
    FOnHealthChanged OnHealthChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Health")
    FOnHealthDepleted OnHealthDepleted;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Health")
    FOnHealthRestored OnHealthRestored;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Health")
    FOnArmorChanged OnArmorChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Health")
    FOnDamageTaken OnDamageTaken;
    
protected:
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // Health settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float MaxHealth;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Health")
    float CurrentHealth;
    
    // Armor settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float MaxArmor;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Health")
    float CurrentArmor;
    
    // Regeneration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    bool bRegenerateHealth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float HealthRegenRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float HealthRegenDelay;
    
    // Damage modifiers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float HeadshotDamageMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float ArmorDamageReduction;
    
    // Invincibility
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    bool bInvincible;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Health")
    float InvincibilityDuration;
    
    // Timers
    UPROPERTY()
    FTimerHandle HealthRegenTimerHandle;
    
    UPROPERTY()
    FTimerHandle LastDamageTimerHandle;
    
    UPROPERTY()
    FTimerHandle InvincibilityTimerHandle;
    
    // Helper functions
    void StartHealthRegen();
    void StopHealthRegen();
    void OnHealthRegenTimer();
    void OnLastDamageTimer();
    void OnInvincibilityTimer();
    
    // Damage calculation
    float CalculateActualDamage(float Damage, FDamageEvent const& DamageEvent);
    
    // Hit detection
    bool IsHeadshot(FDamageEvent const& DamageEvent) const;
};
