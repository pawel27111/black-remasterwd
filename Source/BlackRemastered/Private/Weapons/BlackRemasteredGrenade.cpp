#include "BlackRemasteredGrenade.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Camera/CameraShake.h"
#include "Kismet/GameplayStatics.h"
#include "Core/BlackRemasteredGameState.h"

ABlackRemasteredGrenade::ABlackRemasteredGrenade()
    : Super()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Create collision component
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetSphereRadius(20.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
    CollisionComponent->SetSimulatePhysics(true);
    CollisionComponent->SetLinearDamping(0.1f);
    CollisionComponent->SetAngularDamping(0.1f);
    CollisionComponent->SetMassOverrideInKg(NAME_None, 0.5f);
    
    // Create mesh component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CollisionComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    // Set root component
    RootComponent = CollisionComponent;
    
    // Initialize state
    CurrentState = GS_Held;
    OwnerCharacter = nullptr;
    BounceCount = 0;
    
    // Initialize grenade data with defaults
    GrenadeData.GrenadeName = FName("DefaultGrenade");
    GrenadeData.GrenadeType = GT_Fragmentation;
    GrenadeData.ExplosionRadius = 500.0f;
    GrenadeData.ExplosionDamage = 100.0f;
    GrenadeData.bHurtsOwner = false;
    GrenadeData.FuseTime = 3.0f;
    GrenadeData.ArmTime = 0.5f;
    GrenadeData.ThrowForce = 1500.0f;
    GrenadeData.MaxBounces = 5;
    GrenadeData.BounceDamping = 0.5f;
    GrenadeData.CameraShakeScale = 1.0f;
    
    // Set up collision events
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABlackRemasteredGrenade::OnHit);
}

void ABlackRemasteredGrenade::BeginPlay()
{
    Super::BeginPlay();
    
    // Set up mesh if available
    if (GrenadeData.Mesh)
    {
        MeshComponent->SetStaticMesh(GrenadeData.Mesh);
    }
}

void ABlackRemasteredGrenade::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Handle state updates
    switch (CurrentState)
    {
        case GS_Thrown:
            // Wait for arming
            break;
            
        case GS_Armed:
            // Wait for explosion
            break;
            
        case GS_Exploding:
            // Explosion in progress
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredGrenade::Initialize(const FGrenadeData& Data)
{
    GrenadeData = Data;
    
    // Set mesh if available
    if (GrenadeData.Mesh)
    {
        MeshComponent->SetStaticMesh(GrenadeData.Mesh);
    }
}

void ABlackRemasteredGrenade::Throw(ABlackRemasteredCharacter* Thrower)
{
    OwnerCharacter = Thrower;
    
    // Set state
    SetGrenadeState(GS_Thrown);
    
    // Enable physics
    CollisionComponent->SetSimulatePhysics(true);
    CollisionComponent->SetEnableGravity(true);
    
    // Apply throw force
    if (OwnerCharacter)
    {
        FVector ThrowDirection = OwnerCharacter->GetActorForwardVector();
        ThrowDirection.Z += 0.2f; // Add slight upward angle
        ThrowDirection.Normalize();
        
        CollisionComponent->AddImpulse(ThrowDirection * GrenadeData.ThrowForce);
        
        // Add some random rotation
        CollisionComponent->AddTorqueInRadians(FVector(
            FMath::FRandRange(-1.0f, 1.0f),
            FMath::FRandRange(-1.0f, 1.0f),
            FMath::FRandRange(-1.0f, 1.0f)
        ) * 10.0f);
    }
    
    // Start arm timer
    GetWorld()->GetTimerManager().SetTimer(
        ArmTimerHandle,
        this,
        &ABlackRemasteredGrenade::OnArmTimer,
        GrenadeData.ArmTime,
        false
    );
    
    // Broadcast thrown event
    OnGrenadeThrown.Broadcast();
}

void ABlackRemasteredGrenade::Detonate()
{
    if (CurrentState == GS_Detonated)
    {
        return;
    }
    
    // Set state
    SetGrenadeState(GS_Exploding);
    
    // Explode immediately
    Explode();
}

void ABlackRemasteredGrenade::SetGrenadeState(EGrenadeState NewState)
{
    EGrenadeState OldState = CurrentState;
    CurrentState = NewState;
    
    // Handle state transitions
    switch (NewState)
    {
        case GS_Thrown:
            // Disable collision with owner
            if (OwnerCharacter)
            {
                CollisionComponent->SetCollisionResponseToActor(OwnerCharacter, ECollisionResponse::ECR_Ignore);
            }
            break;
            
        case GS_Armed:
            // Start fuse timer
            GetWorld()->GetTimerManager().SetTimer(
                FuseTimerHandle,
                this,
                &ABlackRemasteredGrenade::OnFuseTimer,
                GrenadeData.FuseTime,
                false
            );
            
            // Broadcast armed event
            OnGrenadeArmed.Broadcast();
            
            // Play arm sound
            if (GrenadeData.ArmSound)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrenadeData.ArmSound, GetActorLocation());
            }
            break;
            
        case GS_Exploding:
            // Explode
            Explode();
            break;
            
        case GS_Detonated:
            // Clean up
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredGrenade::OnFuseTimer()
{
    // Fuse has run out, explode
    SetGrenadeState(GS_Exploding);
}

void ABlackRemasteredGrenade::OnArmTimer()
{
    // Grenade is now armed
    SetGrenadeState(GS_Armed);
}

void ABlackRemasteredGrenade::Explode()
{
    // Set state
    SetGrenadeState(GS_Detonated);
    
    // Apply explosion damage
    ApplyExplosionDamage();
    
    // Apply explosion force
    ApplyExplosionForce();
    
    // Spawn explosion effects
    SpawnExplosionEffects();
    
    // Broadcast exploded event
    OnGrenadeExploded.Broadcast();
    
    // Update game state stats
    if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
    {
        GameState->AddExplosion();
    }
    
    // Destroy self
    Destroy();
}

void ABlackRemasteredGrenade::ApplyExplosionDamage()
{
    // Get all actors in explosion radius
    TArray<AActor*> ActorsToDamage;
    
    // Overlap sphere to find actors
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(GrenadeData.ExplosionRadius),
        QueryParams
    );
    
    // Apply damage to all actors
    for (FOverlapResult& OverlapResult : OverlapResults)
    {
        if (AActor* Actor = OverlapResult.GetActor())
        {
            // Skip owner if grenade doesn't hurt owner
            if (!GrenadeData.bHurtsOwner && Actor == OwnerCharacter)
            {
                continue;
            }
            
            // Calculate distance
            float Distance = FVector::Distance(GetActorLocation(), Actor->GetActorLocation());
            
            // Calculate damage based on distance
            float Damage = GrenadeData.ExplosionDamage * (1.0f - (Distance / GrenadeData.ExplosionRadius));
            Damage = FMath::Max(Damage, 0.0f);
            
            // Apply damage
            UGameplayStatics::ApplyDamage(
                Actor,
                Damage,
                OwnerCharacter ? OwnerCharacter->GetController() : nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }
}

void ABlackRemasteredGrenade::ApplyExplosionForce()
{
    // Get all physics actors in explosion radius
    TArray<AActor*> ActorsToPush;
    
    // Overlap sphere to find actors
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ECC_PhysicsBody,
        FCollisionShape::MakeSphere(GrenadeData.ExplosionRadius),
        QueryParams
    );
    
    // Apply force to all physics actors
    for (FOverlapResult& OverlapResult : OverlapResults)
    {
        if (AActor* Actor = OverlapResult.GetActor())
        {
            if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OverlapResult.GetComponent()))
            {
                if (PrimitiveComponent->IsSimulatingPhysics())
                {
                    // Calculate direction and force
                    FVector Direction = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                    float Distance = FVector::Distance(GetActorLocation(), Actor->GetActorLocation());
                    float ForceScale = 1.0f - (Distance / GrenadeData.ExplosionRadius);
                    ForceScale = FMath::Max(ForceScale, 0.0f);
                    
                    // Apply force
                    PrimitiveComponent->AddImpulse(Direction * 10000.0f * ForceScale);
                }
            }
        }
    }
}

void ABlackRemasteredGrenade::SpawnExplosionEffects()
{
    // Spawn explosion particle effect
    if (GrenadeData.ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            GrenadeData.ExplosionEffect,
            GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(1.0f, 1.0f, 1.0f),
            true,
            EPSCPoolMethod::AutoRelease
        );
    }
    
    // Play explosion sound
    if (GrenadeData.ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            GrenadeData.ExplosionSound,
            GetActorLocation()
        );
    }
    
    // Play camera shake
    if (GrenadeData.ExplosionCameraShake)
    {
        TArray<AActor*> Actors;
        GetWorld()->GetAllActorsOfClass(APlayerController::StaticClass(), Actors);
        
        for (AActor* Actor : Actors)
        {
            if (APlayerController* PlayerController = Cast<APlayerController>(Actor))
            {
                PlayerController->PlayerCameraManager->StartCameraShake(
                    GrenadeData.ExplosionCameraShake,
                    GrenadeData.CameraShakeScale
                );
            }
        }
    }
}

void ABlackRemasteredGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Check if we're armed
    if (CurrentState != GS_Armed && CurrentState != GS_Exploding)
    {
        // Bounce
        OnBounce(Hit);
        return;
    }
    
    // If we hit something while armed, explode
    if (CurrentState == GS_Armed)
    {
        // Check if we hit a valid surface
        if (Hit.IsValidBlockingHit())
        {
            // Explode on impact
            SetGrenadeState(GS_Exploding);
        }
    }
}

void ABlackRemasteredGrenade::OnBounce(const FHitResult& Hit)
{
    // Increment bounce count
    BounceCount++;
    
    // Check if we've bounced too many times
    if (BounceCount >= GrenadeData.MaxBounces)
    {
        // Stop bouncing and arm
        if (CurrentState == GS_Thrown)
        {
            SetGrenadeState(GS_Armed);
        }
        return;
    }
    
    // Play bounce sound
    if (GrenadeData.BounceSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            GrenadeData.BounceSound,
            GetActorLocation()
        );
    }
    
    // Apply physics material
    if (Hit.PhysMaterial.IsValid())
    {
        ApplyPhysicsMaterial(Hit.PhysMaterial.Get());
    }
    
    // Broadcast bounce event
    OnGrenadeBounced.Broadcast();
}

void ABlackRemasteredGrenade::ApplyPhysicsMaterial(UMaterialInterface* Material)
{
    // Apply different physics based on material
    // This is a placeholder for material-specific behavior
    
    // For now, just apply damping based on material
    // In a real implementation, you would check the material type
    // and apply appropriate physics properties
}
