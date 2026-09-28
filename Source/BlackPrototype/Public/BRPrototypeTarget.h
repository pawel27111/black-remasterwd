#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRPrototypeTarget.generated.h"

class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class BLACKPROTOTYPE_API ABRPrototypeTarget : public AActor
{
    GENERATED_BODY()

public:
    ABRPrototypeTarget();
    virtual void Tick(float DeltaSeconds) override;
    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;
    void Configure(bool bIsHostile);

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY()
    TObjectPtr<UStaticMesh> HostileMesh;

    bool bHostile = false;
    float Health = 100.0f;
    float NextShotTime = 0.0f;
};
