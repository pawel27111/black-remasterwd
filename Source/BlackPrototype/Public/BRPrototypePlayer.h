#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BRPrototypePlayer.generated.h"

class UCameraComponent;

UCLASS()
class BLACKPROTOTYPE_API ABRPrototypePlayer : public ACharacter
{
    GENERATED_BODY()

public:
    ABRPrototypePlayer();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

    float GetHealth() const { return Health; }
    int32 GetAmmoInMagazine() const { return AmmoInMagazine; }
    int32 GetReserveAmmo() const { return ReserveAmmo; }
    bool IsReloading() const { return bReloading; }
    void ResetForRound();

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void StartSprint();
    void StopSprint();
    void Fire();
    void Reload();
    void FinishReload();
    void Restart();
    void TogglePause();

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> ViewCamera;

    float Health = 100.0f;
    int32 AmmoInMagazine = 30;
    int32 ReserveAmmo = 90;
    float LastShotTime = -1.0f;
    bool bReloading = false;
    FTimerHandle ReloadTimer;
};
