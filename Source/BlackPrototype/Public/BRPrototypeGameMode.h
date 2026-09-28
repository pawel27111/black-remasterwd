#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BRPrototypeGameMode.generated.h"

class UStaticMesh;

UCLASS()
class BLACKPROTOTYPE_API ABRPrototypeGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABRPrototypeGameMode();
    virtual void StartPlay() override;

    int32 GetRemainingEnemies() const { return RemainingEnemies; }
    void EnemyDestroyed();
    void RestartArena();

private:
    void BuildArena();
    void SpawnWave();
    void SpawnBox(const FVector& Location, const FVector& Dimensions);

    UPROPERTY()
    TObjectPtr<UStaticMesh> BoxMesh;

    int32 RemainingEnemies = 0;
};
