#include "BlackRemasteredDestructionComponent.h"
#include "BlackRemasteredDestructibleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Chaos/ChaosPhysicsComponent.h"
#include "Chaos/ChaosSolversComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Core/BlackRemasteredGameState.h"

UBlackRemasteredDestructionComponent::UBlackRemasteredDestructionComponent()
    : Super()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Initialize state
    OwnerActor = nullptr;
    ChaosPhysicsComponent = nullptr;
    ChaosSolversComponent = nullptr;
    
    // Initialize settings
    DestructionSettings.DestructionMode = DM_Hybrid;
    DestructionSettings.DestructionQuality = DQ_High;
    DestructionSettings.MaxActiveDebris = 100;
    DestructionSettings.MaxFracturePieces = 50;
    DestructionSettings.DebrisCleanupTime = 60.0f;
    DestructionSettings.DebrisCleanupRadius = 1000.0f;
    DestructionSettings.VoxelSize = 4.0f;
    DestructionSettings.VoxelGridSize = 32;
    DestructionSettings.FractureImpulseScale = 1000.0f;
    DestructionSettings.FractureMinChunkSize = 10.0f;
    DestructionSettings.FractureMaxChunkSize = 100.0f;
    DestructionSettings.bEnableChainReactions = true;
    DestructionSettings.ChainReactionRadius = 500.0f;
    DestructionSettings.ChainReactionDelay = 0.1f;
    DestructionSettings.MinDamageForDestruction = 10.0f;
    DestructionSettings.MaxDamagePerHit = 1000.0f;
    DestructionSettings.bEnableDestructionEffects = true;
    DestructionSettings.bEnableDebrisEffects = true;
    DestructionSettings.bEnableSoundEffects = true;
    DestructionSettings.bDebugDestruction = false;
    DestructionSettings.bDebugVoxels = false;
    DestructionSettings.bDebugFractures = false;
    
    // Initialize stats
    DestructionStats.TotalDestructibles = 0;
    DestructionStats.TotalDestroyed = 0;
    DestructionStats.TotalDebrisSpawned = 0;
    DestructionStats.TotalChainReactions = 0;
    DestructionStats.TotalDestructionDamage = 0.0f;
}

void UBlackRemasteredDestructionComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Get owner
    OwnerActor = GetOwner();
    
    // Initialize chaos physics
    InitializeChaosPhysics();
    
    // Initialize destruction settings
    InitializeDestructionSettings();
    
    // Start debris cleanup timer
    GetWorld()->GetTimerManager().SetTimer(
        DebrisCleanupTimerHandle,
        this,
        &UBlackRemasteredDestructionComponent::OnDebrisCleanupTimer,
        DestructionSettings.DebrisCleanupTime,
        true
    );
    
    // Broadcast ready
    OnDestructionSystemReady.Broadcast();
}

void UBlackRemasteredDestructionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Debug drawing
    if (DestructionSettings.bDebugDestruction)
    {
        // Draw debug info
        // This would be visible in the editor
    }
}

void UBlackRemasteredDestructionComponent::Initialize(AActor* Owner)
{
    OwnerActor = Owner;
    
    // Initialize chaos physics
    InitializeChaosPhysics();
    
    // Initialize destruction settings
    InitializeDestructionSettings();
}

void UBlackRemasteredDestructionComponent::ApplyDamageToActor(AActor* Actor, float Damage, EDamageType DamageType, FVector HitLocation, FVector HitNormal)
{
    if (!Actor || !IsActorDestructible(Actor))
    {
        return;
    }
    
    // Clamp damage
    Damage = FMath::Clamp(Damage, 0.0f, DestructionSettings.MaxDamagePerHit);
    
    // Check minimum damage
    if (Damage < DestructionSettings.MinDamageForDestruction)
    {
        return;
    }
    
    // Handle destruction
    HandleDestruction(Actor, Damage, DamageType, HitLocation, HitNormal);
    
    // Update stats
    UpdateDestructionStats(Actor, Damage);
    
    // Broadcast destruction applied
    OnDestructionApplied.Broadcast(Actor);
}

void UBlackRemasteredDestructionComponent::ApplyBallisticDamageToActor(AActor* Actor, float Damage, FVector HitLocation, FVector HitNormal)
{
    ApplyDamageToActor(Actor, Damage, DAM_Ballistic, HitLocation, HitNormal);
}

void UBlackRemasteredDestructionComponent::ApplyExplosiveDamageToActor(AActor* Actor, float Damage, FVector HitLocation)
{
    ApplyDamageToActor(Actor, Damage, DAM_Explosive, HitLocation, FVector::ZeroVector);
}

void UBlackRemasteredDestructionComponent::ApplyFireDamageToActor(AActor* Actor, float Damage, FVector HitLocation)
{
    ApplyDamageToActor(Actor, Damage, DAM_Fire, HitLocation, FVector::ZeroVector);
}

void UBlackRemasteredDestructionComponent::ApplyMeleeDamageToActor(AActor* Actor, float Damage, FVector HitLocation)
{
    ApplyDamageToActor(Actor, Damage, DAM_Melee, HitLocation, FVector::ZeroVector);
}

void UBlackRemasteredDestructionComponent::ApplyVehicleDamageToActor(AActor* Actor, float Damage, FVector HitLocation)
{
    ApplyDamageToActor(Actor, Damage, DAM_Vehicle, HitLocation, FVector::ZeroVector);
}

void UBlackRemasteredDestructionComponent::DestroyActor(AActor* Actor)
{
    if (!Actor || !IsActorDestructible(Actor))
    {
        return;
    }
    
    // Apply max damage to destroy the actor
    float DestroyDamage = DestructionSettings.MaxDamagePerHit * 2.0f;
    ApplyDamageToActor(Actor, DestroyDamage, DAM_Explosive, Actor->GetActorLocation(), FVector::ZeroVector);
}

void UBlackRemasteredDestructionComponent::TriggerChainReaction(AActor* TriggerActor)
{
    if (!DestructionSettings.bEnableChainReactions)
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
            if (Destructible != TriggerActor && Destructible->IsAlive())
            {
                float Distance = FVector::Distance(TriggerActor->GetActorLocation(), Destructible->GetActorLocation());
                
                if (Distance <= DestructionSettings.ChainReactionRadius)
                {
                    // Apply explosive damage
                    float Damage = Destructible->GetMaxHealth() * 0.5f;
                    Destructible->ApplyExplosiveDamage(Damage, TriggerActor->GetActorLocation());
                }
            }
        }
    }
    
    // Broadcast chain reaction started
    OnChainReactionStarted.Broadcast(TriggerActor);
}

bool UBlackRemasteredDestructionComponent::TraceForDestructible(FVector Start, FVector End, FHitResult& HitResult)
{
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(OwnerActor);
    
    // Trace for destructible actors
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        TraceParams
    );
    
    // Check if we hit a destructible actor
    if (bHit)
    {
        if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(HitResult.GetActor()))
        {
            return true;
        }
        
        // Check if the hit component belongs to a destructible actor
        if (HitResult.GetComponent())
        {
            AActor* ComponentOwner = HitResult.GetComponent()->GetOwner();
            if (ComponentOwner)
            {
                if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(ComponentOwner))
                {
                    HitResult.SetActor(Destructible);
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool UBlackRemasteredDestructionComponent::SphereTraceForDestructibles(FVector Center, float Radius, TArray<AActor*>& OutDestructibles)
{
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerActor);
    
    // Overlap sphere
    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        Center,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(Radius),
        QueryParams
    );
    
    if (bHit)
    {
        // Filter for destructible actors
        for (FOverlapResult& OverlapResult : OverlapResults)
        {
            if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(OverlapResult.GetActor()))
            {
                OutDestructibles.AddUnique(Destructible);
            }
        }
    }
    
    return OutDestructibles.Num() > 0;
}

void UBlackRemasteredDestructionComponent::ApplyVoxelDestruction(AActor* Actor, FVector HitLocation, float Radius, float Damage)
{
    if (!Actor || !IsActorDestructible(Actor))
    {
        return;
    }
    
    // Check if actor supports voxel destruction
    if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
    {
        if (Destructible->GetDestructionTier() == DT_Full)
        {
            // Apply voxel-based destruction
            // This would use UE5's Nanite or a custom voxel system
            
            // For now, we'll just apply damage to the actor
            Destructible->ApplyExplosiveDamage(Damage, HitLocation);
        }
    }
}

void UBlackRemasteredDestructionComponent::ApplyFractureDestruction(AActor* Actor, FVector HitLocation, float Damage)
{
    if (!Actor || !IsActorDestructible(Actor))
    {
        return;
    }
    
    // Check if actor supports fracture destruction
    if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
    {
        if (Destructible->GetDestructionTier() == DT_Full || Destructible->GetDestructionTier() == DT_Partial)
        {
            // Apply fracture-based destruction
            // This would use UE5's Chaos Physics fracture system
            
            // For now, we'll just apply damage to the actor
            Destructible->ApplyExplosiveDamage(Damage, HitLocation);
        }
    }
}

void UBlackRemasteredDestructionComponent::ApplySurfaceDamage(AActor* Actor, FVector HitLocation, FVector HitNormal, float Damage)
{
    if (!Actor || !IsActorDestructible(Actor))
    {
        return;
    }
    
    // Check if actor supports surface damage
    if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
    {
        if (Destructible->GetDestructionTier() == DT_Surface || Destructible->GetDestructionTier() == DT_Indestructible)
        {
            // Apply surface damage (decals, etc.)
            Destructible->ApplyBallisticDamage(Damage, HitLocation, HitNormal);
        }
    }
}

void UBlackRemasteredDestructionComponent::SpawnDebris(AActor* Actor, FVector Location, FVector Direction, float Force)
{
    if (!DestructionSettings.bEnableDebrisEffects)
    {
        return;
    }
    
    // Check if we've reached max debris
    if (DestructionStats.TotalDebrisSpawned >= DestructionSettings.MaxActiveDebris)
    {
        // Clean up old debris first
        CleanupDebris();
    }
    
    // Spawn debris actor
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    AActor* DebrisActor = GetWorld()->SpawnActor<AActor>(
        AActor::StaticClass(),
        Location,
        Direction.Rotation(),
        SpawnParams
    );
    
    if (DebrisActor)
    {
        // Add static mesh component to debris actor
        UStaticMeshComponent* DebrisMesh = NewObject<UStaticMeshComponent>(DebrisActor, TEXT("DebrisMesh"));
        DebrisMesh->RegisterComponent();
        DebrisMesh->SetWorldLocation(Location);
        DebrisMesh->SetWorldRotation(Direction.Rotation());
        
        // Set mesh based on actor type
        if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
        {
            // Select random debris mesh from the destructible
            if (Destructible->GetDestructibleData().DebrisMeshes.Num() > 0)
            {
                int32 MeshIndex = FMath::RandRange(0, Destructible->GetDestructibleData().DebrisMeshes.Num() - 1);
                UStaticMeshComponent* SourceMesh = Destructible->GetDestructibleData().DebrisMeshes[MeshIndex];
                
                if (SourceMesh && SourceMesh->GetStaticMesh())
                {
                    DebrisMesh->SetStaticMesh(SourceMesh->GetStaticMesh());
                }
            }
        }
        
        // Enable physics
        DebrisMesh->SetSimulatePhysics(true);
        DebrisMesh->SetEnableGravity(true);
        DebrisMesh->SetLinearDamping(0.1f);
        DebrisMesh->SetAngularDamping(0.1f);
        
        // Apply impulse
        DebrisMesh->AddImpulse(Direction * Force);
        
        // Set lifecycle
        DebrisActor->SetLifeSpan(DestructionSettings.DebrisCleanupTime);
        
        // Update stats
        DestructionStats.TotalDebrisSpawned++;
    }
}

void UBlackRemasteredDestructionComponent::CleanupDebris()
{
    // Find all debris actors in the world
    TArray<AActor*> AllDebris;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllDebris);
    
    // This is a simplified cleanup
    // In a real implementation, you would track all spawned debris
    // and clean up the oldest ones when the limit is reached
}

bool UBlackRemasteredDestructionComponent::IsDestructionEnabled() const
{
    return DestructionSettings.DestructionMode != DM_None;
}

bool UBlackRemasteredDestructionComponent::CanDestroyActor(AActor* Actor) const
{
    if (!Actor || !IsActorDestructible(Actor))
    {
        return false;
    }
    
    // Check destruction tier
    EDestructionTier Tier = GetActorDestructionTier(Actor);
    
    return Tier != DT_Indestructible;
}

// Timer callback
void UBlackRemasteredDestructionComponent::OnDebrisCleanupTimer()
{
    CleanupDebris();
}

// Helper functions
void UBlackRemasteredDestructionComponent::InitializeChaosPhysics()
{
    // Get or create chaos physics component
    if (!ChaosPhysicsComponent)
    {
        ChaosPhysicsComponent = NewObject<UChaosPhysicsComponent>(OwnerActor, TEXT("ChaosPhysicsComponent"));
    }
    
    // Get or create chaos solvers component
    if (!ChaosSolversComponent)
    {
        ChaosSolversComponent = NewObject<UChaosSolversComponent>(OwnerActor, TEXT("ChaosSolversComponent"));
    }
}

void UBlackRemasteredDestructionComponent::InitializeDestructionSettings()
{
    // Initialize settings based on quality level
    switch (DestructionSettings.DestructionQuality)
    {
        case DQ_Low:
            DestructionSettings.MaxActiveDebris = 50;
            DestructionSettings.MaxFracturePieces = 20;
            DestructionSettings.VoxelSize = 8.0f;
            DestructionSettings.VoxelGridSize = 16;
            break;
            
        case DQ_Medium:
            DestructionSettings.MaxActiveDebris = 100;
            DestructionSettings.MaxFracturePieces = 50;
            DestructionSettings.VoxelSize = 4.0f;
            DestructionSettings.VoxelGridSize = 32;
            break;
            
        case DQ_High:
            DestructionSettings.MaxActiveDebris = 200;
            DestructionSettings.MaxFracturePieces = 100;
            DestructionSettings.VoxelSize = 2.0f;
            DestructionSettings.VoxelGridSize = 64;
            break;
            
        case DQ_Ultra:
            DestructionSettings.MaxActiveDebris = 500;
            DestructionSettings.MaxFracturePieces = 200;
            DestructionSettings.VoxelSize = 1.0f;
            DestructionSettings.VoxelGridSize = 128;
            break;
            
        default:
            break;
    }
}

void UBlackRemasteredDestructionComponent::HandleDestruction(AActor* Actor, float Damage, EDamageType DamageType, FVector HitLocation, FVector HitNormal)
{
    if (!Actor)
    {
        return;
    }
    
    // Check if this is a destructible actor
    if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
    {
        // Apply damage based on destruction tier
        switch (Destructible->GetDestructionTier())
        {
            case DT_Full:
                // Full destruction - use voxel or fracture
                if (DestructionSettings.DestructionMode == DM_Voxel || DestructionSettings.DestructionMode == DM_Hybrid)
                {
                    ApplyVoxelDestruction(Actor, HitLocation, 50.0f, Damage);
                }
                
                if (DestructionSettings.DestructionMode == DM_Fracture || DestructionSettings.DestructionMode == DM_Hybrid)
                {
                    ApplyFractureDestruction(Actor, HitLocation, Damage);
                }
                break;
                
            case DT_Partial:
                // Partial destruction - use fracture
                ApplyFractureDestruction(Actor, HitLocation, Damage);
                break;
                
            case DT_Surface:
                // Surface damage - use decals
                ApplySurfaceDamage(Actor, HitLocation, HitNormal, Damage);
                break;
                
            default:
                // Indestructible - do nothing
                break;
        }
    }
    else
    {
        // Check if the actor has a static mesh component
        if (UStaticMeshComponent* StaticMesh = Actor->FindComponentByClass<UStaticMeshComponent>())
        {
            // Apply surface damage to static mesh
            ApplySurfaceDamage(Actor, HitLocation, HitNormal, Damage);
        }
    }
}

void UBlackRemasteredDestructionComponent::UpdateDestructionStats(AActor* Actor, float Damage)
{
    // Update total damage
    DestructionStats.TotalDestructionDamage += Damage;
    
    // Check if actor was destroyed
    if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
    {
        if (Destructible->IsDestroyed())
        {
            DestructionStats.TotalDestroyed++;
        }
    }
}

void UBlackRemasteredDestructionComponent::CheckForSupportingStructures(AActor* DestroyedActor)
{
    if (!DestroyedActor)
    {
        return;
    }
    
    // Check if this was a supporting structure
    if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(DestroyedActor))
    {
        if (Destructible->IsSupportingStructure())
        {
            // Find all actors that were supported by this structure
            // In a real implementation, you would have a system to track supporting structures
            
            // For now, we'll just check for actors above the destroyed actor
            TArray<AActor*> AllActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
            
            for (AActor* Actor : AllActors)
            {
                if (Actor != DestroyedActor)
                {
                    // Check if actor is above the destroyed actor
                    float ActorZ = Actor->GetActorLocation().Z;
                    float DestroyedZ = DestroyedActor->GetActorLocation().Z;
                    
                    if (ActorZ > DestroyedZ)
                    {
                        // Check if actor is within a certain horizontal distance
                        FVector HorizontalDistance = FVector::Distance2D(Actor->GetActorLocation(), DestroyedActor->GetActorLocation());
                        
                        if (HorizontalDistance < 500.0f)
                        {
                            // This actor might fall
                            if (ABlackRemasteredDestructibleActor* OtherDestructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
                            {
                                // Apply some damage to simulate falling
                                OtherDestructible->ApplyExplosiveDamage(500.0f, DestroyedActor->GetActorLocation());
                            }
                        }
                    }
                }
            }
        }
    }
}

bool UBlackRemasteredDestructionComponent::IsActorDestructible(AActor* Actor) const
{
    if (!Actor)
    {
        return false;
    }
    
    // Check if actor is a destructible actor
    if (Cast<ABlackRemasteredDestructibleActor>(Actor))
    {
        return true;
    }
    
    // Check if actor has a static mesh component
    if (Actor->FindComponentByClass<UStaticMeshComponent>())
    {
        return true;
    }
    
    return false;
}

EDestructionTier UBlackRemasteredDestructionComponent::GetActorDestructionTier(AActor* Actor) const
{
    if (!Actor)
    {
        return DT_Indestructible;
    }
    
    // Check if actor is a destructible actor
    if (ABlackRemasteredDestructibleActor* Destructible = Cast<ABlackRemasteredDestructibleActor>(Actor))
    {
        return Destructible->GetDestructionTier();
    }
    
    // Default to surface damage for static mesh actors
    return DT_Surface;
}
