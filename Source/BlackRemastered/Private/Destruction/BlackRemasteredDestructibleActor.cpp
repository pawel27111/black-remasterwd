#include "BlackRemasteredDestructibleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Chaos/ChaosPhysicsComponent.h"
#include "Chaos/ChaosSolversComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Core/BlackRemasteredGameState.h"

ABlackRemasteredDestructibleActor::ABlackRemasteredDestructibleActor()
    : Super()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Create mesh component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    
    // Create chaos physics component
    ChaosPhysicsComponent = CreateDefaultSubobject<UChaosPhysicsComponent>(TEXT("ChaosPhysicsComponent"));
    
    // Initialize state
    CurrentState = DS_Intact;
    
    // Initialize destructible data with defaults
    DestructibleData.DestructibleName = FName("DefaultDestructible");
    DestructibleData.DestructibleType = DT_Concrete;
    DestructibleData.DestructionTier = DT_Partial;
    DestructibleData.MaxHealth = 1000.0f;
    DestructibleData.CurrentHealth = 1000.0f;
    DestructibleData.BallisticDamageMultiplier = 1.0f;
    DestructibleData.ExplosiveDamageMultiplier = 1.5f;
    DestructibleData.FireDamageMultiplier = 0.5f;
    DestructibleData.MeleeDamageMultiplier = 0.8f;
    DestructibleData.VehicleDamageMultiplier = 2.0f;
    DestructibleData.DamagedThreshold = 0.7f;
    DestructibleData.BrokenThreshold = 0.4f;
    DestructibleData.DestroyedThreshold = 0.1f;
    DestructibleData.bUseChaosPhysics = true;
    DestructibleData.Mass = 100.0f;
    DestructibleData.LinearDamping = 0.1f;
    DestructibleData.AngularDamping = 0.1f;
    DestructibleData.bFractureOnDestruction = true;
    DestructibleData.FracturePiecesCount = 10;
    DestructibleData.FractureImpulseScale = 1000.0f;
    DestructibleData.MinDebrisCount = 5;
    DestructibleData.MaxDebrisCount = 15;
    DestructibleData.DebrisVelocityScale = 500.0f;
    DestructibleData.bCanTriggerChainReaction = false;
    DestructibleData.ChainReactionRadius = 300.0f;
    DestructibleData.ChainReactionDamage = 500.0f;
    DestructibleData.bExplodeOnDestruction = false;
    DestructibleData.ExplosionRadius = 500.0f;
    DestructibleData.ExplosionDamage = 1000.0f;
    DestructibleData.ExplosionForce = 5000.0f;
    DestructibleData.DestructionXP = 50;
}

void ABlackRemasteredDestructibleActor::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize chaos physics
    if (DestructibleData.bUseChaosPhysics && ChaosPhysicsComponent)
    {
        ChaosPhysicsComponent->SetMassOverride(DestructibleData.Mass);
        ChaosPhysicsComponent->SetLinearDamping(DestructibleData.LinearDamping);
        ChaosPhysicsComponent->SetAngularDamping(DestructibleData.AngularDamping);
        ChaosPhysicsComponent->SetEnableGravity(true);
        ChaosPhysicsComponent->SetSimulatePhysics(false); // Start disabled
    }
    
    // Initialize chunks if we have a fractured mesh
    // This would be set up in the editor or via data asset
}

void ABlackRemasteredDestructibleActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update state based on health
    UpdateState();
}

void ABlackRemasteredDestructibleActor::Initialize(const FDestructibleData& Data)
{
    DestructibleData = Data;
    CurrentState = DS_Intact;
    
    // Set mesh if available
    if (DestructibleData.Mesh)
    {
        MeshComponent->SetStaticMesh(DestructibleData.Mesh);
    }
    
    // Initialize chaos physics
    if (DestructibleData.bUseChaosPhysics && ChaosPhysicsComponent)
    {
        ChaosPhysicsComponent->SetMassOverride(DestructibleData.Mass);
        ChaosPhysicsComponent->SetLinearDamping(DestructibleData.LinearDamping);
        ChaosPhysicsComponent->SetAngularDamping(DestructibleData.AngularDamping);
    }
}

void ABlackRemasteredDestructibleActor::ApplyDamage(float Damage, EDamageType DamageType, FVector HitLocation, FVector HitNormal)
{
    if (CurrentState == DS_Destroyed)
    {
        return;
    }
    
    // Apply damage multiplier based on damage type
    float EffectiveDamage = Damage;
    
    switch (DamageType)
    {
        case DAM_Ballistic:
            EffectiveDamage *= DestructibleData.BallisticDamageMultiplier;
            break;
            
        case DAM_Explosive:
            EffectiveDamage *= DestructibleData.ExplosiveDamageMultiplier;
            break;
            
        case DAM_Fire:
            EffectiveDamage *= DestructibleData.FireDamageMultiplier;
            break;
            
        case DAM_Melee:
            EffectiveDamage *= DestructibleData.MeleeDamageMultiplier;
            break;
            
        case DAM_Vehicle:
            EffectiveDamage *= DestructibleData.VehicleDamageMultiplier;
            break;
            
        default:
            break;
    }
    
    // Apply damage to health
    DestructibleData.CurrentHealth = FMath::Max(DestructibleData.CurrentHealth - EffectiveDamage, 0.0f);
    
    // Apply damage effects
    ApplyDamageEffects(EffectiveDamage, DamageType, HitLocation);
    
    // Broadcast damage applied
    OnDamageApplied.Broadcast(EffectiveDamage, DamageType, HitLocation);
    
    // Update state
    UpdateState();
    
    // Check for chain reaction
    CheckForChainReaction();
}

void ABlackRemasteredDestructibleActor::ApplyBallisticDamage(float Damage, FVector HitLocation, FVector HitNormal)
{
    ApplyDamage(Damage, DAM_Ballistic, HitLocation, HitNormal);
}

void ABlackRemasteredDestructibleActor::ApplyExplosiveDamage(float Damage, FVector HitLocation)
{
    ApplyDamage(Damage, DAM_Explosive, HitLocation, FVector::ZeroVector);
}

void ABlackRemasteredDestructibleActor::ApplyFireDamage(float Damage, FVector HitLocation)
{
    ApplyDamage(Damage, DAM_Fire, HitLocation, FVector::ZeroVector);
}

void ABlackRemasteredDestructibleActor::ApplyMeleeDamage(float Damage, FVector HitLocation)
{
    ApplyDamage(Damage, DAM_Melee, HitLocation, FVector::ZeroVector);
}

void ABlackRemasteredDestructibleActor::ApplyVehicleDamage(float Damage, FVector HitLocation)
{
    ApplyDamage(Damage, DAM_Vehicle, HitLocation, FVector::ZeroVector);
}

void ABlackRemasteredDestructibleActor::Destroy()
{
    if (CurrentState == DS_Destroyed)
    {
        return;
    }
    
    // Set state
    SetDestructibleState(DS_Destroyed);
    
    // Apply destruction effects
    ApplyDestructionEffects();
    
    // Fracture if enabled
    if (DestructibleData.bFractureOnDestruction)
    {
        Fracture();
    }
    
    // Spawn debris
    SpawnDebris();
    
    // Explode if enabled
    if (DestructibleData.bExplodeOnDestruction)
    {
        Explode();
    }
    
    // Trigger chain reaction
    if (DestructibleData.bCanTriggerChainReaction)
    {
        TriggerChainReaction();
    }
    
    // Award XP
    if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
    {
        // XP is awarded to player in GameState
    }
    
    // Broadcast destroyed
    OnDestroyed.Broadcast();
    
    // Set lifecycle
    SetLifeSpan(30.0f); // Remove after 30 seconds
}

void ABlackRemasteredDestructibleActor::Fracture()
{
    if (Chunks.Num() == 0)
    {
        return;
    }
    
    // Enable physics on all chunks
    for (FDestructibleChunk& Chunk : Chunks)
    {
        if (Chunk.Mesh)
        {
            // Detach chunk
            Chunk.Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            
            // Enable physics
            Chunk.Mesh->SetSimulatePhysics(true);
            Chunk.Mesh->SetEnableGravity(true);
            Chunk.Mesh->SetLinearDamping(DestructibleData.LinearDamping);
            Chunk.Mesh->SetAngularDamping(DestructibleData.AngularDamping);
            
            // Apply impulse
            FVector ImpulseDirection = FVector::UpVector + FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
            ImpulseDirection.Normalize();
            
            Chunk.Mesh->AddImpulse(ImpulseDirection * DestructibleData.FractureImpulseScale);
            
            // Mark as fractured
            Chunk.bIsFractured = true;
        }
    }
    
    // Hide original mesh
    MeshComponent->SetVisibility(false);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABlackRemasteredDestructibleActor::SpawnDebris()
{
    if (DestructibleData.DebrisMeshes.Num() == 0)
    {
        return;
    }
    
    // Spawn random number of debris pieces
    int32 DebrisCount = FMath::RandRange(DestructibleData.MinDebrisCount, DestructibleData.MaxDebrisCount);
    
    for (int32 i = 0; i < DebrisCount; i++)
    {
        // Select random debris mesh
        int32 MeshIndex = FMath::RandRange(0, DestructibleData.DebrisMeshes.Num() - 1);
        UStaticMeshComponent* DebrisMesh = DestructibleData.DebrisMeshes[MeshIndex];
        
        if (!DebrisMesh)
        {
            continue;
        }
        
        // Spawn debris actor
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
        AActor* DebrisActor = GetWorld()->SpawnActor<AActor>(
            AActor::StaticClass(),
            GetActorLocation() + FVector(FMath::FRandRange(-50.0f, 50.0f), FMath::FRandRange(-50.0f, 50.0f), FMath::FRandRange(0.0f, 50.0f)),
            GetActorRotation(),
            SpawnParams
        );
        
        if (DebrisActor)
        {
            // Add static mesh component to debris actor
            UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(DebrisActor, TEXT("DebrisMesh"));
            NewMesh->RegisterComponent();
            NewMesh->SetStaticMesh(DebrisMesh->GetStaticMesh());
            NewMesh->SetWorldTransform(DebrisMesh->GetComponentTransform());
            NewMesh->SetSimulatePhysics(true);
            NewMesh->SetEnableGravity(true);
            
            // Apply random impulse
            FVector ImpulseDirection = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(0.5f, 1.0f)).GetSafeNormal();
            NewMesh->AddImpulse(ImpulseDirection * DestructibleData.DebrisVelocityScale);
            
            // Add to spawned debris list
            SpawnedDebris.Add(DebrisActor);
            
            // Set lifecycle
            DebrisActor->SetLifeSpan(60.0f);
        }
    }
}

void ABlackRemasteredDestructibleActor::TriggerChainReaction()
{
    if (!DestructibleData.bCanTriggerChainReaction)
    {
        return;
    }
    
    // Find all destructible actors in chain reaction radius
    TArray<AActor*> AllDestructibles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlackRemasteredDestructibleActor::StaticClass(), AllDestructibles);
    
    for (AActor* Actor : AllDestructibles)
    {
        if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
        {
            if (Destructible != this && Destructible->IsAlive())
            {
                float Distance = FVector::Distance(GetActorLocation(), Destructible->GetActorLocation());
                
                if (Distance <= DestructibleData.ChainReactionRadius)
                {
                    // Apply damage to this destructible
                    Destructible->ApplyExplosiveDamage(DestructibleData.ChainReactionDamage, GetActorLocation());
                }
            }
        }
    }
    
    // Broadcast chain reaction triggered
    OnChainReactionTriggered.Broadcast();
}

void ABlackRemasteredDestructibleActor::Explode()
{
    // Apply explosion damage to all actors in radius
    TArray<AActor*> AllActors;
    
    // Overlap sphere to find actors
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(DestructibleData.ExplosionRadius),
        QueryParams
    );
    
    // Apply damage and force to all actors
    for (FOverlapResult& OverlapResult : OverlapResults)
    {
        if (AActor* Actor = OverlapResult.GetActor())
        {
            // Calculate distance
            float Distance = FVector::Distance(GetActorLocation(), Actor->GetActorLocation());
            
            // Calculate damage based on distance
            float Damage = DestructibleData.ExplosionDamage * (1.0f - (Distance / DestructibleData.ExplosionRadius));
            Damage = FMath::Max(Damage, 0.0f);
            
            // Apply damage
            UGameplayStatics::ApplyDamage(
                Actor,
                Damage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
            
            // Apply force to physics actors
            if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OverlapResult.GetComponent()))
            {
                if (PrimitiveComponent->IsSimulatingPhysics())
                {
                    FVector Direction = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                    float ForceScale = 1.0f - (Distance / DestructibleData.ExplosionRadius);
                    ForceScale = FMath::Max(ForceScale, 0.0f);
                    
                    PrimitiveComponent->AddImpulse(Direction * DestructibleData.ExplosionForce * ForceScale);
                }
            }
        }
    }
    
    // Spawn explosion effects
    if (DestructibleData.DestructionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            DestructibleData.DestructionEffect,
            GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(DestructibleData.ExplosionRadius * 0.1f, DestructibleData.ExplosionRadius * 0.1f, DestructibleData.ExplosionRadius * 0.1f),
            true,
            EPSCPoolMethod::AutoRelease
        );
    }
    
    // Play explosion sound
    if (DestructibleData.DestructionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            DestructibleData.DestructionSound,
            GetActorLocation()
        );
    }
    
    // Update game state stats
    if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
    {
        GameState->AddExplosion();
    }
}

void ABlackRemasteredDestructibleActor::SetDestructibleState(EDestructibleState NewState)
{
    EDestructibleState OldState = CurrentState;
    CurrentState = NewState;
    
    // Handle state transitions
    switch (NewState)
    {
        case DS_Intact:
            // Reset health
            DestructibleData.CurrentHealth = DestructibleData.MaxHealth;
            
            // Reset mesh visibility
            MeshComponent->SetVisibility(true);
            MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            
            // Disable physics
            if (ChaosPhysicsComponent)
            {
                ChaosPhysicsComponent->SetSimulatePhysics(false);
            }
            break;
            
        case DS_Damaged:
            // Apply damage effects
            break;
            
        case DS_Broken:
            // Enable physics
            if (ChaosPhysicsComponent)
            {
                ChaosPhysicsComponent->SetSimulatePhysics(true);
            }
            break;
            
        case DS_Destroyed:
            // Trigger destruction
            Destroy();
            break;
            
        default:
            break;
    }
    
    // Broadcast state change
    OnStateChanged.Broadcast(OldState, NewState);
}

void ABlackRemasteredDestructibleActor::Repair(float Amount)
{
    if (CurrentState == DS_Destroyed)
    {
        return;
    }
    
    DestructibleData.CurrentHealth = FMath::Min(DestructibleData.CurrentHealth + Amount, DestructibleData.MaxHealth);
    
    // Update state
    UpdateState();
}

void ABlackRemasteredDestructibleActor::FullyRepair()
{
    DestructibleData.CurrentHealth = DestructibleData.MaxHealth;
    
    // Reset state
    SetDestructibleState(DS_Intact);
}

float ABlackRemasteredDestructibleActor::GetHealthPercentage() const
{
    if (DestructibleData.MaxHealth <= 0.0f)
    {
        return 0.0f;
    }
    return DestructibleData.CurrentHealth / DestructibleData.MaxHealth * 100.0f;
}

bool ABlackRemasteredDestructibleActor::CanBeDestroyed() const
{
    return DestructibleData.DestructionTier != DT_Indestructible;
}

bool ABlackRemasteredDestructibleActor::CanTriggerChainReaction() const
{
    return DestructibleData.bCanTriggerChainReaction && CurrentState != DS_Destroyed;
}

// Helper functions
void ABlackRemasteredDestructibleActor::OnChainReactionTimer()
{
    // Chain reaction timer expired
    // This is used for delayed chain reactions
}

void ABlackRemasteredDestructibleActor::UpdateState()
{
    if (CurrentState == DS_Destroyed)
    {
        return;
    }
    
    // Calculate health percentage
    float HealthPercentage = GetHealthPercentage() / 100.0f;
    
    // Determine state based on health percentage and thresholds
    if (HealthPercentage <= DestructibleData.DestroyedThreshold)
    {
        SetDestructibleState(DS_Destroyed);
    }
    else if (HealthPercentage <= DestructibleData.BrokenThreshold)
    {
        SetDestructibleState(DS_Broken);
    }
    else if (HealthPercentage <= DestructibleData.DamagedThreshold)
    {
        SetDestructibleState(DS_Damaged);
    }
    else
    {
        SetDestructibleState(DS_Intact);
    }
}

void ABlackRemasteredDestructibleActor::SpawnChunk(FDestructibleChunk& ChunkData)
{
    // This would spawn a chunk actor with physics
    // For now, we'll just enable physics on the chunk mesh
}

void ABlackRemasteredDestructibleActor::ApplyDamageEffects(float Damage, EDamageType DamageType, FVector HitLocation)
{
    // Spawn damage particles
    if (DestructibleData.DamageEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            DestructibleData.DamageEffect,
            HitLocation,
            FRotator::ZeroRotator,
            FVector(1.0f, 1.0f, 1.0f),
            true,
            EPSCPoolMethod::AutoRelease
        );
    }
    
    // Play damage sound
    if (DestructibleData.DamageSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            DestructibleData.DamageSound,
            HitLocation
        );
    }
    
    // Apply damage decals
    ApplyDamageDecals(Damage, DamageType, HitLocation, FVector::ZeroVector);
}

void ABlackRemasteredDestructibleActor::ApplyDestructionEffects()
{
    // Spawn destruction particles
    if (DestructibleData.DestructionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            DestructibleData.DestructionEffect,
            GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(1.0f, 1.0f, 1.0f),
            true,
            EPSCPoolMethod::AutoRelease
        );
    }
    
    // Play destruction sound
    if (DestructibleData.DestructionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            DestructibleData.DestructionSound,
            GetActorLocation()
        );
    }
}

void ABlackRemasteredDestructibleActor::ApplyDamageDecals(float Damage, EDamageType DamageType, FVector HitLocation, FVector HitNormal)
{
    // Apply decal based on damage type
    // This would spawn a decal actor at the hit location
    
    // For now, we'll just trace for the surface and apply a decal
    // In a real implementation, you would have different decals for each damage type
}

void ABlackRemasteredDestructibleActor::CheckForChainReaction()
{
    // Check if we should trigger chain reaction
    if (DestructibleData.bCanTriggerChainReaction && CurrentState == DS_Destroyed)
    {
        // Start chain reaction timer
        GetWorld()->GetTimerManager().SetTimer(
            ChainReactionTimerHandle,
            this,
            &ABlackRemasteredDestructibleActor::TriggerChainReaction,
            0.1f,
            false
        );
    }
}

void ABlackRemasteredDestructibleActor::ApplyExplosionForce(FVector ExplosionLocation, float Force)
{
    if (!MeshComponent || !MeshComponent->IsSimulatingPhysics())
    {
        return;
    }
    
    // Calculate direction and distance
    FVector Direction = (GetActorLocation() - ExplosionLocation).GetSafeNormal();
    float Distance = FVector::Distance(GetActorLocation(), ExplosionLocation);
    
    // Calculate force based on distance
    float ForceScale = 1.0f - (Distance / 1000.0f);
    ForceScale = FMath::Max(ForceScale, 0.0f);
    
    // Apply force
    MeshComponent->AddImpulse(Direction * Force * ForceScale);
}

bool ABlackRemasteredDestructibleActor::IsSupportingStructure() const
{
    // Check if this is a supporting structure
    // This would be determined by the level designer
    return false;
}
