#include "BlackRemasteredEnemyCharacter.h"
#include "BlackRemasteredAIController.h"
#include "Gameplay/BlackRemasteredHealthComponent.h"
#include "Gameplay/BlackRemasteredWeaponComponent.h"
#include "BlackRemasteredCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ABlackRemasteredEnemyCharacter::ABlackRemasteredEnemyCharacter()
    : Super()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Initialize state
    CurrentState = ES_Idle;
    CurrentBehavior = EB_Idle;
    TargetCharacter = nullptr;
    EnemyAIController = nullptr;
    
    // Create components
    HealthComponent = CreateDefaultSubobject<UBlackRemasteredHealthComponent>(TEXT("HealthComponent"));
    WeaponComponent = CreateDefaultSubobject<UBlackRemasteredWeaponComponent>(TEXT("WeaponComponent"));
    
    // Initialize enemy data with defaults
    EnemyData.EnemyName = FName("DefaultEnemy");
    EnemyData.EnemyType = ET_Grunt;
    EnemyData.Difficulty = ED_Veteran;
    EnemyData.MaxHealth = 100.0f;
    EnemyData.CurrentHealth = 100.0f;
    EnemyData.MaxArmor = 0.0f;
    EnemyData.CurrentArmor = 0.0f;
    EnemyData.HeadshotDamageMultiplier = 2.0f;
    EnemyData.ArmorDamageReduction = 0.5f;
    EnemyData.WalkSpeed = 200.0f;
    EnemyData.RunSpeed = 300.0f;
    EnemyData.SprintSpeed = 400.0f;
    EnemyData.CrouchSpeed = 150.0f;
    EnemyData.SightRadius = 2000.0f;
    EnemyData.SightAngle = 180.0f;
    EnemyData.HearingRadius = 1000.0f;
    EnemyData.MemoryTime = 10.0f;
    EnemyData.Accuracy = 0.8f;
    EnemyData.FireRate = 0.2f;
    EnemyData.ReloadTime = 2.0f;
    EnemyData.BurstCount = 3;
    EnemyData.SuppressionRadius = 500.0f;
    EnemyData.FlankDistance = 500.0f;
    EnemyData.RetreatHealthThreshold = 0.3f;
    EnemyData.ReactionTime = 0.5f;
    EnemyData.DecisionInterval = 0.5f;
    EnemyData.Aggression = 0.7f;
    EnemyData.Caution = 0.3f;
    EnemyData.bCanUseGrenades = false;
    EnemyData.GrenadeCooldown = 10.0f;
    EnemyData.bSpawnWithWeapon = true;
    EnemyData.bSpawnWithGrenades = false;
    EnemyData.KillXP = 100;
    EnemyData.HeadshotXP = 150;
    
    // Set default weapon
    EnemyData.AvailableWeapons = { WT_AssaultRifle };
    EnemyData.CurrentWeaponType = WT_AssaultRifle;
}

void ABlackRemasteredEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize components
    if (HealthComponent)
    {
        HealthComponent->Initialize(this);
        HealthComponent->SetMaxHealth(EnemyData.MaxHealth);
        HealthComponent->SetCurrentHealth(EnemyData.CurrentHealth);
        HealthComponent->SetMaxArmor(EnemyData.MaxArmor);
        HealthComponent->SetCurrentArmor(EnemyData.CurrentArmor);
    }
    
    // Initialize weapon component
    if (WeaponComponent)
    {
        WeaponComponent->Initialize(this);
    }
    
    // Set up character movement
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = EnemyData.WalkSpeed;
        GetCharacterMovement()->MaxWalkSpeedCrouched = EnemyData.CrouchSpeed;
    }
    
    // Get AI controller
    EnemyAIController = Cast<ABlackRemasteredAIController>(GetController());
    
    // Start timers
    GetWorld()->GetTimerManager().SetTimer(
        BehaviorTimerHandle,
        this,
        &ABlackRemasteredEnemyCharacter::OnBehaviorTimer,
        1.0f,
        true
    );
    
    GetWorld()->GetTimerManager().SetTimer(
        DecisionTimerHandle,
        this,
        &ABlackRemasteredEnemyCharacter::OnDecisionTimer,
        EnemyData.DecisionInterval,
        true
    );
    
    GetWorld()->GetTimerManager().SetTimer(
        PerceptionTimerHandle,
        this,
        &ABlackRemasteredEnemyCharacter::OnPerceptionTimer,
        0.1f,
        true
    );
}

void ABlackRemasteredEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update AI
    UpdateAI(DeltaTime);
    
    // Update perception
    UpdatePerception(DeltaTime);
}

void ABlackRemasteredEnemyCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    
    // Get AI controller
    EnemyAIController = Cast<ABlackRemasteredAIController>(NewController);
    
    // Initialize AI
    if (EnemyAIController)
    {
        EnemyAIController->InitializeEnemy(this);
    }
}

void ABlackRemasteredEnemyCharacter::Initialize(const FEnemyData& Data)
{
    EnemyData = Data;
    
    // Initialize health
    if (HealthComponent)
    {
        HealthComponent->SetMaxHealth(EnemyData.MaxHealth);
        HealthComponent->SetCurrentHealth(EnemyData.CurrentHealth);
        HealthComponent->SetMaxArmor(EnemyData.MaxArmor);
        HealthComponent->SetCurrentArmor(EnemyData.CurrentArmor);
    }
    
    // Initialize movement
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = EnemyData.WalkSpeed;
        GetCharacterMovement()->MaxWalkSpeedCrouched = EnemyData.CrouchSpeed;
    }
    
    // Set state
    CurrentState = ES_Idle;
    CurrentBehavior = EB_Idle;
}

void ABlackRemasteredEnemyCharacter::Fire()
{
    if (!WeaponComponent || CurrentState == ES_Dead)
    {
        return;
    }
    
    // Check if we can fire
    if (WeaponComponent->CanFire())
    {
        WeaponComponent->Fire();
        
        // Play fire sound
        // TODO: Play weapon-specific fire sound
    }
}

void ABlackRemasteredEnemyCharacter::Reload()
{
    if (!WeaponComponent || CurrentState == ES_Dead)
    {
        return;
    }
    
    // Check if we can reload
    if (WeaponComponent->CanReload())
    {
        WeaponComponent->Reload();
        
        // Play reload sound
        // TODO: Play weapon-specific reload sound
    }
}

void ABlackRemasteredEnemyCharacter::ThrowGrenade()
{
    if (CurrentState == ES_Dead || !EnemyData.bCanUseGrenades)
    {
        return;
    }
    
    // TODO: Implement grenade throwing
    // - Check if we have grenades
    // - Throw grenade towards target
    // - Start cooldown
}

void ABlackRemasteredEnemyCharacter::MeleeAttack()
{
    if (!WeaponComponent || CurrentState == ES_Dead)
    {
        return;
    }
    
    // Check if we can melee
    if (WeaponComponent->CanMelee())
    {
        WeaponComponent->Melee();
    }
}

void ABlackRemasteredEnemyCharacter::MoveToLocation(const FVector& Location)
{
    if (!EnemyAIController)
    {
        return;
    }
    
    // Move to location using AI controller
    EnemyAIController->MoveToLocation(Location);
}

void ABlackRemasteredEnemyCharacter::MoveToCover()
{
    if (!EnemyAIController)
    {
        return;
    }
    
    // Find cover and move to it
    FindCover();
    
    if (CanTakeCover())
    {
        // TODO: Find nearest cover and move to it
        // EnemyAIController->MoveToCover();
    }
}

void ABlackRemasteredEnemyCharacter::TakeCover()
{
    if (CurrentState == ES_Dead)
    {
        return;
    }
    
    // Set state
    SetEnemyState(ES_InCover);
    
    // TODO: Play cover animation
}

void ABlackRemasteredEnemyCharacter::Flank()
{
    if (!EnemyAIController || !TargetCharacter)
    {
        return;
    }
    
    // Find flank position
    FVector FlankPosition;
    FindFlankPosition(FlankPosition);
    
    // Move to flank position
    MoveToLocation(FlankPosition);
    
    // Set behavior
    SetEnemyBehavior(EB_Flank);
}

void ABlackRemasteredEnemyCharacter::Retreat()
{
    if (!EnemyAIController)
    {
        return;
    }
    
    // Find retreat position (away from target)
    if (TargetCharacter)
    {
        FVector RetreatDirection = (GetActorLocation() - TargetCharacter->GetActorLocation()).GetSafeNormal();
        FVector RetreatPosition = GetActorLocation() + (RetreatDirection * 500.0f);
        
        // Move to retreat position
        MoveToLocation(RetreatPosition);
    }
    
    // Set behavior
    SetEnemyBehavior(EB_Retreat);
}

void ABlackRemasteredEnemyCharacter::DetectPlayer()
{
    if (CurrentState == ES_Dead)
    {
        return;
    }
    
    // Find player
    FindTarget();
    
    if (TargetCharacter)
    {
        // Set state based on current behavior
        if (CurrentBehavior == EB_Idle || CurrentBehavior == EB_Patrol)
        {
            SetEnemyBehavior(EB_Alert);
        }
        else if (CurrentBehavior == EB_Alert || CurrentBehavior == EB_Search)
        {
            SetEnemyBehavior(EB_Engage);
        }
    }
}

void ABlackRemasteredEnemyCharacter::LosePlayer()
{
    if (TargetCharacter)
    {
        TargetCharacter = nullptr;
        
        // Set behavior based on current state
        if (CurrentBehavior == EB_Engage || CurrentBehavior == EB_Suppress)
        {
            SetEnemyBehavior(EB_Search);
        }
    }
}

void ABlackRemasteredEnemyCharacter::HearNoise(const FVector& NoiseLocation, float NoiseLoudness)
{
    if (CurrentState == ES_Dead)
    {
        return;
    }
    
    // Check if noise is within hearing radius
    float DistanceToNoise = FVector::Distance(GetActorLocation(), NoiseLocation);
    
    if (DistanceToNoise <= EnemyData.HearingRadius)
    {
        // Check if noise is loud enough
        float VolumeAtDistance = NoiseLoudness / (1.0f + DistanceToNoise * 0.1f);
        
        if (VolumeAtDistance > 0.5f) // Threshold
        {
            // Investigate noise
            if (CurrentBehavior == EB_Idle || CurrentBehavior == EB_Patrol)
            {
                SetEnemyBehavior(EB_Investigate);
                MoveToLocation(NoiseLocation);
            }
            else if (CurrentBehavior == EB_Alert || CurrentBehavior == EB_Search)
            {
                // Already investigating, just update target location
                MoveToLocation(NoiseLocation);
            }
        }
    }
}

void ABlackRemasteredEnemyCharacter::SeeDeadBody(AActor* DeadBody)
{
    if (CurrentState == ES_Dead)
    {
        return;
    }
    
    // Check if dead body is an ally
    // TODO: Implement ally detection
    
    // Increase aggression
    EnemyData.Aggression = FMath::Min(EnemyData.Aggression + 0.2f, 1.0f);
    
    // Investigate dead body
    if (CurrentBehavior == EB_Idle || CurrentBehavior == EB_Patrol)
    {
        SetEnemyBehavior(EB_Investigate);
        MoveToLocation(DeadBody->GetActorLocation());
    }
}

void ABlackRemasteredEnemyCharacter::SetEnemyState(EEnemyState NewState)
{
    EEnemyState OldState = CurrentState;
    CurrentState = NewState;
    
    // Handle state transitions
    switch (NewState)
    {
        case ES_Idle:
            // Reset behavior if not already set
            if (CurrentBehavior == EB_None)
            {
                SetEnemyBehavior(EB_Idle);
            }
            break;
            
        case ES_Alert:
            // Start alert behavior
            if (CurrentBehavior != EB_Engage && CurrentBehavior != EB_Suppress)
            {
                SetEnemyBehavior(EB_Alert);
            }
            break;
            
        case ES_Engaged:
            // Start engage behavior
            SetEnemyBehavior(EB_Engage);
            break;
            
        case ES_Damaged:
            // React to damage
            break;
            
        case ES_Dead:
            // Handle death
            if (EnemyAIController)
            {
                EnemyAIController->StopMovement();
            }
            break;
            
        default:
            break;
    }
    
    // Broadcast state change
    OnEnemyStateChanged.Broadcast(OldState, NewState);
}

void ABlackRemasteredEnemyCharacter::SetEnemyBehavior(EEnemyBehavior NewBehavior)
{
    EEnemyBehavior OldBehavior = CurrentBehavior;
    CurrentBehavior = NewBehavior;
    
    // Handle behavior transitions
    switch (NewBehavior)
    {
        case EB_Idle:
            // Stop any movement
            if (EnemyAIController)
            {
                EnemyAIController->StopMovement();
            }
            break;
            
        case EB_Patrol:
            // Start patrolling
            if (EnemyAIController)
            {
                EnemyAIController->StartPatrol();
            }
            break;
            
        case EB_Alert:
            // Alert other enemies
            // TODO: Implement alert system
            break;
            
        case EB_Search:
            // Start searching
            if (EnemyAIController)
            {
                EnemyAIController->StartSearch();
            }
            break;
            
        case EB_Investigate:
            // Investigate specific location
            break;
            
        case EB_Engage:
            // Start engaging target
            if (EnemyAIController)
            {
                EnemyAIController->StartEngage();
            }
            break;
            
        case EB_Suppress:
            // Start suppressing
            if (EnemyAIController)
            {
                EnemyAIController->StartSuppress();
            }
            break;
            
        case EB_Flank:
            // Start flanking
            if (EnemyAIController)
            {
                EnemyAIController->StartFlank();
            }
            break;
            
        case EB_Retreat:
            // Start retreating
            if (EnemyAIController)
            {
                EnemyAIController->StartRetreat();
            }
            break;
            
        case EB_Regroup:
            // Start regrouping
            if (EnemyAIController)
            {
                EnemyAIController->StartRegroup();
            }
            break;
            
        case EB_Cover:
            // Take cover
            TakeCover();
            break;
            
        default:
            break;
    }
    
    // Broadcast behavior change
    OnEnemyBehaviorChanged.Broadcast(OldBehavior, NewBehavior);
}

void ABlackRemasteredEnemyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (CurrentState == ES_Dead)
    {
        return;
    }
    
    // Apply damage to health component
    if (HealthComponent)
    {
        HealthComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    }
    
    // Update enemy data health
    EnemyData.CurrentHealth = HealthComponent ? HealthComponent->GetCurrentHealth() : 0.0f;
    
    // Broadcast damage taken
    OnEnemyTookDamage.Broadcast(Damage, EventInstigator, DamageCauser);
    
    // Check for death
    if (EnemyData.CurrentHealth <= 0.0f)
    {
        Die(EventInstigator);
    }
    else
    {
        // React to damage
        SetEnemyState(ES_Damaged);
        
        // Check if we should retreat
        if (EnemyData.CurrentHealth / EnemyData.MaxHealth <= EnemyData.RetreatHealthThreshold)
        {
            if (FMath::FRand() < EnemyData.Caution)
            {
                Retreat();
            }
        }
    }
}

void ABlackRemasteredEnemyCharacter::Die(AController* Killer)
{
    // Set state
    SetEnemyState(ES_Dead);
    
    // Disable movement
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
    }
    
    // Disable collision
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    
    // Stop AI
    if (EnemyAIController)
    {
        EnemyAIController->StopAI();
    }
    
    // Broadcast death
    OnEnemyDied.Broadcast(Killer);
    
    // Award XP to killer
    if (Killer)
    {
        // TODO: Award XP to player
    }
    
    // Play death animation
    // TODO: Play random death animation
    
    // Set lifecycle
    SetLifeSpan(10.0f); // Remove after 10 seconds
}

float ABlackRemasteredEnemyCharacter::GetHealthPercentage() const
{
    if (EnemyData.MaxHealth <= 0.0f)
    {
        return 0.0f;
    }
    return EnemyData.CurrentHealth / EnemyData.MaxHealth * 100.0f;
}

bool ABlackRemasteredEnemyCharacter::HasLineOfSight() const
{
    if (!TargetCharacter)
    {
        return false;
    }
    
    // Trace from enemy to target
    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this);
    TraceParams.AddIgnoredActor(TargetCharacter);
    
    FVector StartLocation = GetActorLocation();
    StartLocation.Z += 50.0f; // Raise to eye level
    
    FVector EndLocation = TargetCharacter->GetActorLocation();
    EndLocation.Z += 50.0f;
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        TraceParams
    );
    
    // If we hit something, check if it's the target
    if (bHit)
    {
        return HitResult.GetActor() == TargetCharacter;
    }
    
    return true;
}

ABlackRemasteredAIController* ABlackRemasteredEnemyCharacter::GetEnemyAIController() const
{
    return EnemyAIController;
}

// Timer callbacks
void ABlackRemasteredEnemyCharacter::OnBehaviorTimer()
{
    // Update behavior based on state
    switch (CurrentState)
    {
        case ES_Damaged:
            // Return to previous state after damage
            SetEnemyState(ES_Engaged);
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredEnemyCharacter::OnDecisionTimer()
{
    // Make AI decision
    MakeDecision();
}

void ABlackRemasteredEnemyCharacter::OnPerceptionTimer()
{
    // Update perception
    UpdatePerception(0.1f);
}

// AI Update
void ABlackRemasteredEnemyCharacter::UpdateAI(float DeltaTime)
{
    if (CurrentState == ES_Dead)
    {
        return;
    }
    
    // Update target
    UpdateTarget();
    
    // Make decisions based on current behavior
    switch (CurrentBehavior)
    {
        case EB_Idle:
            // Check for player
            DetectPlayer();
            break;
            
        case EB_Patrol:
            // Check for player while patrolling
            DetectPlayer();
            break;
            
        case EB_Alert:
            // Try to find player
            DetectPlayer();
            break;
            
        case EB_Search:
            // Search for player
            DetectPlayer();
            break;
            
        case EB_Investigate:
            // Investigate noise/location
            break;
            
        case EB_Engage:
            // Engage target
            if (TargetCharacter && HasLineOfSight())
            {
                // Fire at target
                if (FMath::FRand() < EnemyData.Accuracy)
                {
                    Fire();
                }
                
                // Check if we should take cover
                if (FMath::FRand() < EnemyData.Caution * 0.5f)
                {
                    MoveToCover();
                }
                
                // Check if we should flank
                if (FMath::FRand() < EnemyData.Aggression * 0.3f)
                {
                    Flank();
                }
            }
            else
            {
                // Lost target
                LosePlayer();
            }
            break;
            
        case EB_Suppress:
            // Suppress target area
            if (TargetCharacter)
            {
                // Fire in direction of target
                Fire();
            }
            break;
            
        case EB_Flank:
            // Continue flanking
            break;
            
        case EB_Retreat:
            // Continue retreating
            break;
            
        case EB_Regroup:
            // Regroup with other enemies
            break;
            
        case EB_Cover:
            // Stay in cover
            if (TargetCharacter && HasLineOfSight())
            {
                // Fire from cover
                if (FMath::FRand() < EnemyData.Accuracy * 0.7f)
                {
                    Fire();
                }
            }
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredEnemyCharacter::MakeDecision()
{
    if (CurrentState == ES_Dead)
    {
        return;
    }
    
    // Make decision based on current state and behavior
    switch (CurrentBehavior)
    {
        case EB_Idle:
        case EB_Patrol:
            // Randomly switch between idle and patrol
            if (FMath::FRand() < 0.1f)
            {
                if (CurrentBehavior == EB_Idle)
                {
                    SetEnemyBehavior(EB_Patrol);
                }
                else
                {
                    SetEnemyBehavior(EB_Idle);
                }
            }
            break;
            
        case EB_Alert:
            // Switch to search if we've been alert for a while
            if (FMath::FRand() < 0.3f)
            {
                SetEnemyBehavior(EB_Search);
            }
            break;
            
        case EB_Search:
            // Switch back to patrol if we haven't found anything
            if (FMath::FRand() < 0.2f)
            {
                SetEnemyBehavior(EB_Patrol);
            }
            break;
            
        case EB_Engage:
            // Randomly switch to suppress or flank
            if (FMath::FRand() < 0.2f)
            {
                if (FMath::FRand() < 0.5f)
                {
                    SetEnemyBehavior(EB_Suppress);
                }
                else
                {
                    SetEnemyBehavior(EB_Flank);
                }
            }
            break;
            
        case EB_Suppress:
            // Switch back to engage
            if (FMath::FRand() < 0.3f)
            {
                SetEnemyBehavior(EB_Engage);
            }
            break;
            
        case EB_Flank:
            // Switch back to engage after flanking
            if (FMath::FRand() < 0.5f)
            {
                SetEnemyBehavior(EB_Engage);
            }
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredEnemyCharacter::UpdatePerception(float DeltaTime)
{
    if (CurrentState == ES_Dead)
    {
        return;
    }
    
    // Check for player in sight
    if (!TargetCharacter)
    {
        FindTarget();
    }
    else
    {
        UpdateTarget();
    }
    
    // Check for noises
    // TODO: Implement noise detection system
}

void ABlackRemasteredEnemyCharacter::FindTarget()
{
    if (TargetCharacter)
    {
        return;
    }
    
    // Get all player characters
    TArray<AActor*> PlayerCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlackRemasteredCharacter::StaticClass(), PlayerCharacters);
    
    // Find closest player
    float MinDistance = TNumericLimits<float>::Max();
    ABlackRemasteredCharacter* ClosestPlayer = nullptr;
    
    for (AActor* Actor : PlayerCharacters)
    {
        if (ABlackRemasteredCharacter* Player = Cast<ABlackRemasteredCharacter>(Actor))
        {
            if (Player->IsAlive())
            {
                float Distance = FVector::Distance(GetActorLocation(), Player->GetActorLocation());
                
                if (Distance < MinDistance && Distance <= EnemyData.SightRadius)
                {
                    // Check angle
                    FVector DirectionToPlayer = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                    FVector EnemyForward = GetActorForwardVector();
                    
                    float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(DirectionToPlayer, EnemyForward)));
                    
                    if (Angle <= EnemyData.SightAngle * 0.5f)
                    {
                        // Check line of sight
                        FHitResult HitResult;
                        FCollisionQueryParams TraceParams;
                        TraceParams.AddIgnoredActor(this);
                        TraceParams.AddIgnoredActor(Player);
                        
                        FVector StartLocation = GetActorLocation();
                        StartLocation.Z += 50.0f;
                        
                        FVector EndLocation = Player->GetActorLocation();
                        EndLocation.Z += 50.0f;
                        
                        bool bHit = GetWorld()->LineTraceSingleByChannel(
                            HitResult,
                            StartLocation,
                            EndLocation,
                            ECC_Visibility,
                            TraceParams
                        );
                        
                        if (!bHit || HitResult.GetActor() == Player)
                        {
                            MinDistance = Distance;
                            ClosestPlayer = Player;
                        }
                    }
                }
            }
        }
    }
    
    // Set target
    if (ClosestPlayer)
    {
        TargetCharacter = ClosestPlayer;
        DetectPlayer();
    }
}

void ABlackRemasteredEnemyCharacter::UpdateTarget()
{
    if (!TargetCharacter || !TargetCharacter->IsAlive())
    {
        TargetCharacter = nullptr;
        LosePlayer();
        return;
    }
    
    // Check if target is still in range
    float DistanceToTarget = FVector::Distance(GetActorLocation(), TargetCharacter->GetActorLocation());
    
    if (DistanceToTarget > EnemyData.SightRadius * 1.5f)
    {
        // Target is too far, lose it
        TargetCharacter = nullptr;
        LosePlayer();
    }
    else if (DistanceToTarget <= EnemyData.SightRadius)
    {
        // Target is in range, check line of sight
        if (!HasLineOfSight())
        {
            // Lost line of sight, start searching
            if (CurrentBehavior != EB_Search && CurrentBehavior != EB_Investigate)
            {
                SetEnemyBehavior(EB_Search);
            }
        }
    }
}

void ABlackRemasteredEnemyCharacter::FindCover()
{
    // TODO: Implement cover finding
    // - Trace in multiple directions for cover
    // - Find nearest valid cover
    // - Store cover location
}

bool ABlackRemasteredEnemyCharacter::CanTakeCover() const
{
    // TODO: Implement cover checking
    return false;
}

void ABlackRemasteredEnemyCharacter::FindFlankPosition(FVector& OutPosition)
{
    if (!TargetCharacter)
    {
        OutPosition = GetActorLocation();
        return;
    }
    
    // Calculate position to flank target
    FVector TargetLocation = TargetCharacter->GetActorLocation();
    FVector EnemyLocation = GetActorLocation();
    
    // Calculate direction to target
    FVector DirectionToTarget = (TargetLocation - EnemyLocation).GetSafeNormal();
    
    // Calculate flank direction (perpendicular to target direction)
    FVector FlankDirection = FVector::CrossProduct(DirectionToTarget, FVector::UpVector).GetSafeNormal();
    
    // Randomly choose left or right flank
    if (FMath::FRand() < 0.5f)
    {
        FlankDirection = -FlankDirection;
    }
    
    // Calculate flank position
    OutPosition = EnemyLocation + (FlankDirection * EnemyData.FlankDistance);
    
    // Add some randomness
    OutPosition.X += FMath::FRandRange(-50.0f, 50.0f);
    OutPosition.Y += FMath::FRandRange(-50.0f, 50.0f);
}

void ABlackRemasteredEnemyCharacter::CheckForObstacles()
{
    // TODO: Implement obstacle detection
    // - Trace forward for obstacles
    // - If obstacle found, avoid it
}

void ABlackRemasteredEnemyCharacter::AvoidObstacle()
{
    // TODO: Implement obstacle avoidance
    // - Calculate new direction to avoid obstacle
    // - Move in new direction
}

void ABlackRemasteredEnemyCharacter::PlayFootstepSound()
{
    // TODO: Implement footstep sounds
    // - Play sound based on surface material
    // - Volume based on movement speed
}

void ABlackRemasteredEnemyCharacter::PlayVoiceLine(EEnemyState State)
{
    // TODO: Implement voice lines
    // - Play random voice line based on state
    // - Different lines for different enemy types
}
