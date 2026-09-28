#include "BRPrototypeGameMode.h"
#include "BRPrototypeHUD.h"
#include "BRPrototypePlayer.h"
#include "BRPrototypeTarget.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABRPrototypeGameMode::ABRPrototypeGameMode()
{
    DefaultPawnClass = ABRPrototypePlayer::StaticClass();
    HUDClass = ABRPrototypeHUD::StaticClass();

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Box(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Box.Succeeded())
    {
        BoxMesh = Box.Object;
    }
}

void ABRPrototypeGameMode::StartPlay()
{
    Super::StartPlay();
    BuildArena();
    SpawnWave();

    if (ABRPrototypePlayer* Player = Cast<ABRPrototypePlayer>(UGameplayStatics::GetPlayerPawn(this, 0)))
    {
        Player->ResetForRound();
    }
}

void ABRPrototypeGameMode::SpawnBox(const FVector& Location, const FVector& Dimensions)
{
    if (!BoxMesh)
    {
        return;
    }

    AStaticMeshActor* Box = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
    if (Box)
    {
        UStaticMeshComponent* Component = Box->GetStaticMeshComponent();
        Component->SetMobility(EComponentMobility::Movable);
        Component->SetStaticMesh(BoxMesh);
        Component->SetCollisionProfileName(TEXT("BlockAll"));
        Box->SetActorScale3D(Dimensions / 100.0f);
    }
}

void ABRPrototypeGameMode::BuildArena()
{
    // The engine's Entry map provides a blank world; these are temporary assets.
    SpawnBox(FVector(0, 0, -50), FVector(4400, 3200, 100));
    SpawnBox(FVector(0, -1600, 200), FVector(4400, 100, 400));
    SpawnBox(FVector(0, 1600, 200), FVector(4400, 100, 400));
    SpawnBox(FVector(-2200, 0, 200), FVector(100, 3200, 400));
    SpawnBox(FVector(2200, 0, 200), FVector(100, 3200, 400));

    ADirectionalLight* Sun = GetWorld()->SpawnActor<ADirectionalLight>(
        FVector(0, 0, 900), FRotator(-55, -35, 0));
    if (Sun)
    {
        Sun->GetLightComponent()->SetIntensity(5.0f);
    }

    ASkyLight* Ambient = GetWorld()->SpawnActor<ASkyLight>();
    if (Ambient)
    {
        Ambient->GetLightComponent()->SetIntensity(1.0f);
    }
}

void ABRPrototypeGameMode::SpawnWave()
{
    RemainingEnemies = 0;

    const FVector EnemyPositions[] = {
        FVector(1000, -850, 90), FVector(1450, -330, 90),
        FVector(1050, 350, 90), FVector(1450, 850, 90)
    };

    for (const FVector& Position : EnemyPositions)
    {
        if (ABRPrototypeTarget* Enemy = GetWorld()->SpawnActor<ABRPrototypeTarget>(Position, FRotator::ZeroRotator))
        {
            Enemy->Configure(true);
            ++RemainingEnemies;
        }
    }

    const FVector CoverPositions[] = {
        FVector(420, -430, 75), FVector(620, 160, 75), FVector(810, 690, 75)
    };

    for (const FVector& Position : CoverPositions)
    {
        if (ABRPrototypeTarget* Cover = GetWorld()->SpawnActor<ABRPrototypeTarget>(Position, FRotator::ZeroRotator))
        {
            Cover->Configure(false);
        }
    }
}

void ABRPrototypeGameMode::EnemyDestroyed()
{
    RemainingEnemies = FMath::Max(0, RemainingEnemies - 1);
}

void ABRPrototypeGameMode::RestartArena()
{
    TArray<AActor*> Targets;
    UGameplayStatics::GetAllActorsOfClass(this, ABRPrototypeTarget::StaticClass(), Targets);
    for (AActor* Target : Targets)
    {
        Target->Destroy();
    }

    SpawnWave();
    if (ABRPrototypePlayer* Player = Cast<ABRPrototypePlayer>(UGameplayStatics::GetPlayerPawn(this, 0)))
    {
        Player->ResetForRound();
    }
}
