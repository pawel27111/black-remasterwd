#include "BRPrototypeTarget.h"
#include "BRPrototypeGameMode.h"
#include "BRPrototypePlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABRPrototypeTarget::ABRPrototypeTarget()
{
    PrimaryActorTick.bCanEverTick = true;
    SetCanBeDamaged(true);
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetCollisionProfileName(TEXT("BlockAll"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (Cube.Succeeded())
    {
        Mesh->SetStaticMesh(Cube.Object);
    }
    if (Cylinder.Succeeded())
    {
        HostileMesh = Cylinder.Object;
    }
}

void ABRPrototypeTarget::Configure(bool bIsHostile)
{
    bHostile = bIsHostile;
    Health = bHostile ? 100.0f : 135.0f;
    if (bHostile && HostileMesh)
    {
        Mesh->SetStaticMesh(HostileMesh);
    }
    SetActorScale3D(bHostile ? FVector(0.8f, 0.8f, 1.7f) : FVector(2.2f, 1.3f, 1.5f));
    NextShotTime = GetWorld()->GetTimeSeconds() + FMath::FRandRange(0.8f, 1.8f);
}

void ABRPrototypeTarget::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bHostile || GetWorld()->GetTimeSeconds() < NextShotTime)
    {
        return;
    }

    ABRPrototypePlayer* Player = Cast<ABRPrototypePlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (!Player)
    {
        return;
    }

    const FVector Start = GetActorLocation() + FVector(0, 0, 35);
    const FVector End = Player->GetActorLocation() + FVector(0, 0, 65);
    if (FVector::DistSquared(Start, End) > FMath::Square(2800.0f))
    {
        return;
    }

    // The covers block the same visibility trace used by player fire.
    FCollisionQueryParams Query(SCENE_QUERY_STAT(PrototypeEnemySight), true, this);
    FHitResult Hit;
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Query)
        && Hit.GetActor() == Player)
    {
        UGameplayStatics::ApplyDamage(Player, 7.0f, nullptr, this, UDamageType::StaticClass());
    }
    NextShotTime = GetWorld()->GetTimeSeconds() + 1.25f;
}

float ABRPrototypeTarget::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Health -= Applied;
    if (Health <= 0.0f)
    {
        if (bHostile)
        {
            if (ABRPrototypeGameMode* Mode = GetWorld()->GetAuthGameMode<ABRPrototypeGameMode>())
            {
                Mode->EnemyDestroyed();
            }
        }
        Destroy();
    }
    return Applied;
}
