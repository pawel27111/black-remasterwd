#include "BlackRemasteredAIController.h"
#include "BlackRemasteredEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

ABlackRemasteredAIController::ABlackRemasteredAIController()
    : Super()
{
    // Set up perception component
    SetupPerceptionSystem();
    
    // Initialize state
    EnemyCharacter = nullptr;
    bIsInCover = false;
    bIsEngaged = false;
    bIsFlanking = false;
    bIsRetreating = false;
    
    // Initialize AI data
    AIData.SightRadius = 2000.0f;
    AIData.SightAngle = 180.0f;
    AIData.HearingRadius = 1000.0f;
    AIData.MemoryTime = 10.0f;
    AIData.MoveSpeed = 200.0f;
    AIData.RotationSpeed = 5.0f;
    AIData.AttackRange = 500.0f;
    AIData.AttackCooldown = 0.5f;
    AIData.Accuracy = 0.8f;
    AIData.CoverSearchRadius = 500.0f;
    AIData.CoverMinHeight = 100.0f;
    AIData.FlankDistance = 500.0f;
    AIData.FlankAngle = 45.0f;
    AIData.RetreatDistance = 1000.0f;
    AIData.RetreatHealthThreshold = 0.3f;
    
    // Blackboard key names
    TargetKeyName = "Target";
    TargetLocationKeyName = "TargetLocation";
    CoverLocationKeyName = "CoverLocation";
    BehaviorKeyName = "Behavior";
    StateKeyName = "State";
}

void ABlackRemasteredAIController::BeginPlay()
{
    Super::BeginPlay();
    
    // Set up behavior tree
    if (AIData.BehaviorTree)
    {
        RunBehaviorTree(AIData.BehaviorTree);
        AIData.Blackboard = GetBlackboardComponent();
    }
}

void ABlackRemasteredAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update AI
    if (EnemyCharacter && EnemyCharacter->IsAlive())
    {
        UpdatePerception();
        UpdateTarget();
        UpdateBehavior();
    }
}

void ABlackRemasteredAIController::Possess(APawn* InPawn)
{
    Super::Possess(InPawn);
    
    // Get enemy character
    EnemyCharacter = Cast<ABlackRemasteredEnemyCharacter>(InPawn);
    
    // Initialize enemy
    if (EnemyCharacter)
    {
        InitializeEnemy(EnemyCharacter);
    }
    
    // Set up behavior tree
    if (AIData.BehaviorTree)
    {
        RunBehaviorTree(AIData.BehaviorTree);
        AIData.Blackboard = GetBlackboardComponent();
    }
    
    // Set up perception
    SetupPerceptionSystem();
}

void ABlackRemasteredAIController::UnPossess()
{
    // Clean up
    EnemyCharacter = nullptr;
    
    Super::UnPossess();
}

void ABlackRemasteredAIController::SetupPerceptionSystem()
{
    // Create perception component if it doesn't exist
    if (!AIData.PawnSensing)
    {
        AIData.PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
        AIData.PawnSensing->SetPeripheralVisionAngle(AIData.SightAngle);
        AIData.PawnSensing->SetSightRadius(AIData.SightRadius);
        AIData.PawnSensing->SetHearingThreshold(1.0f);
        AIData.PawnSensing->SetLOSHearingThreshold(1.0f);
    }
    
    // Set up AI perception component
    UAIPerceptionComponent* AIPerception = GetAIPerceptionComponent();
    if (AIPerception)
    {
        // Configure sight
        UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
        SightConfig->SightRadius = AIData.SightRadius;
        SightConfig->LoseSightRadius = AIData.SightRadius * 1.2f;
        SightConfig->PeripheralVisionAngleDegrees = AIData.SightAngle;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        
        // Configure hearing
        UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
        HearingConfig->HearingRange = AIData.HearingRadius;
        HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
        HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
        HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
        
        // Set perception configurations
        AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
        AIPerception->ConfigureSense(*SightConfig);
        AIPerception->ConfigureSense(*HearingConfig);
        
        // Set perception update callback
        AIPerception->OnPerceptionUpdated.AddDynamic(this, &ABlackRemasteredAIController::OnPerceptionUpdated);
    }
}

void ABlackRemasteredAIController::InitializeEnemy(ABlackRemasteredEnemyCharacter* Enemy)
{
    EnemyCharacter = Enemy;
    
    // Update AI data from enemy
    if (EnemyCharacter)
    {
        AIData.SightRadius = EnemyCharacter->GetEnemyData().SightRadius;
        AIData.SightAngle = EnemyCharacter->GetEnemyData().SightAngle;
        AIData.HearingRadius = EnemyCharacter->GetEnemyData().HearingRadius;
        AIData.MoveSpeed = EnemyCharacter->GetEnemyData().WalkSpeed;
        AIData.AttackRange = 500.0f; // Default
        AIData.Accuracy = EnemyCharacter->GetEnemyData().Accuracy;
        
        // Set up perception with enemy-specific values
        SetupPerceptionSystem();
    }
}

void ABlackRemasteredAIController::MoveToLocation(const FVector& Location)
{
    if (!GetPathFollowingComponent() || !EnemyCharacter)
    {
        return;
    }
    
    // Stop current movement
    StopMovement();
    
    // Move to location
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(nullptr);
    MoveRequest.SetGoalLocation(Location);
    MoveRequest.SetAcceptanceRadius(50.0f); // Accept when within 50 units
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAllowPartialPath(true);
    MoveRequest.SetProjectGoalOnNavigation(true);
    
    GetPathFollowingComponent()->RequestMove(MoveRequest, nullptr);
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsVector(TargetLocationKeyName, Location);
    }
}

void ABlackRemasteredAIController::MoveToActor(AActor* Actor)
{
    if (!Actor || !GetPathFollowingComponent())
    {
        return;
    }
    
    // Stop current movement
    StopMovement();
    
    // Move to actor
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(Actor);
    MoveRequest.SetAcceptanceRadius(50.0f);
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAllowPartialPath(true);
    
    GetPathFollowingComponent()->RequestMove(MoveRequest, nullptr);
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsObject(TargetKeyName, Actor);
    }
}

void ABlackRemasteredAIController::StopMovement()
{
    if (GetPathFollowingComponent())
    {
        GetPathFollowingComponent()->StopMovement();
    }
}

void ABlackRemasteredAIController::SetMoveSpeed(float Speed)
{
    AIData.MoveSpeed = Speed;
    
    if (EnemyCharacter && EnemyCharacter->GetCharacterMovement())
    {
        EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed;
    }
}

void ABlackRemasteredAIController::StartPatrol()
{
    if (!EnemyCharacter || bIsEngaged)
    {
        return;
    }
    
    // Set up patrol points if not already set up
    SetupPatrolPoints();
    
    // Move to first patrol point
    MoveToNextPatrolPoint();
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsEnum(BehaviorKeyName, static_cast<uint8>(EB_Patrol));
    }
}

void ABlackRemasteredAIController::StartSearch()
{
    if (!EnemyCharacter || bIsEngaged)
    {
        return;
    }
    
    // Search around last known target location
    if (CurrentTargetInfo.TargetActor)
    {
        SearchArea(CurrentTargetInfo.LastKnownLocation, 500.0f);
    }
    else
    {
        // Search around current location
        SearchArea(GetPawn()->GetActorLocation(), 300.0f);
    }
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsEnum(BehaviorKeyName, static_cast<uint8>(EB_Search));
    }
}

void ABlackRemasteredAIController::StartEngage()
{
    if (!EnemyCharacter || !CurrentTargetInfo.TargetActor)
    {
        return;
    }
    
    bIsEngaged = true;
    
    // Move towards target
    MoveToActor(CurrentTargetInfo.TargetActor);
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsEnum(BehaviorKeyName, static_cast<uint8>(EB_Engage));
        AIData.Blackboard->SetValueAsObject(TargetKeyName, CurrentTargetInfo.TargetActor);
    }
    
    // Start attack timer
    GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &ABlackRemasteredAIController::OnAttackTimer,
        AIData.AttackCooldown,
        true
    );
}

void ABlackRemasteredAIController::StartSuppress()
{
    if (!EnemyCharacter || !CurrentTargetInfo.TargetActor)
    {
        return;
    }
    
    bIsEngaged = true;
    
    // Find cover to suppress from
    FindCover();
    
    if (bIsInCover)
    {
        // Suppress from cover
        SuppressArea(CurrentTargetInfo.LastKnownLocation);
    }
    else
    {
        // Suppress from current location
        SuppressArea(CurrentTargetInfo.TargetActor->GetActorLocation());
    }
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsEnum(BehaviorKeyName, static_cast<uint8>(EB_Suppress));
    }
}

void ABlackRemasteredAIController::StartFlank()
{
    if (!EnemyCharacter || !CurrentTargetInfo.TargetActor)
    {
        return;
    }
    
    bIsFlanking = true;
    bIsEngaged = true;
    
    // Find flank position
    FVector FlankPosition;
    FindBestFlankPosition(FlankPosition);
    
    // Move to flank position
    MoveToLocation(FlankPosition);
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsEnum(BehaviorKeyName, static_cast<uint8>(EB_Flank));
    }
}

void ABlackRemasteredAIController::StartRetreat()
{
    if (!EnemyCharacter)
    {
        return;
    }
    
    bIsRetreating = true;
    
    // Find retreat position
    FVector RetreatPosition = GetPawn()->GetActorLocation();
    
    if (CurrentTargetInfo.TargetActor)
    {
        // Retreat away from target
        FVector RetreatDirection = (GetPawn()->GetActorLocation() - CurrentTargetInfo.TargetActor->GetActorLocation()).GetSafeNormal();
        RetreatPosition = GetPawn()->GetActorLocation() + (RetreatDirection * AIData.RetreatDistance);
    }
    else
    {
        // Retreat in random direction
        FVector RetreatDirection = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
        RetreatPosition = GetPawn()->GetActorLocation() + (RetreatDirection * AIData.RetreatDistance);
    }
    
    // Move to retreat position
    MoveToLocation(RetreatPosition);
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsEnum(BehaviorKeyName, static_cast<uint8>(EB_Retreat));
    }
}

void ABlackRemasteredAIController::StartRegroup()
{
    if (!EnemyCharacter)
    {
        return;
    }
    
    // Find regroup position
    FVector RegroupPosition;
    FindBestRegroupPosition(RegroupPosition);
    
    // Move to regroup position
    MoveToLocation(RegroupPosition);
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsEnum(BehaviorKeyName, static_cast<uint8>(EB_Regroup));
    }
}

void ABlackRemasteredAIController::MoveToCover()
{
    if (!EnemyCharacter || bIsInCover)
    {
        return;
    }
    
    // Find cover
    FindCover();
    
    if (bIsInCover)
    {
        // Move to cover location
        MoveToLocation(CurrentCoverInfo.CoverLocation);
    }
}

void ABlackRemasteredAIController::UseGrenade()
{
    if (!EnemyCharacter || !CurrentTargetInfo.TargetActor)
    {
        return;
    }
    
    // Throw grenade at target
    ThrowGrenadeAtLocation(CurrentTargetInfo.TargetActor->GetActorLocation());
}

void ABlackRemasteredAIController::SetTarget(AActor* Target)
{
    if (Target == CurrentTargetInfo.TargetActor)
    {
        return;
    }
    
    AActor* OldTarget = CurrentTargetInfo.TargetActor;
    CurrentTargetInfo.TargetActor = Target;
    CurrentTargetInfo.TimeSinceLastSeen = 0.0f;
    
    if (Target)
    {
        CurrentTargetInfo.LastKnownLocation = Target->GetActorLocation();
        CurrentTargetInfo.Distance = FVector::Distance(GetPawn()->GetActorLocation(), Target->GetActorLocation());
        CurrentTargetInfo.bIsVisible = true;
        CurrentTargetInfo.bHasLineOfSight = HasLineOfSightTo(Target);
    }
    else
    {
        CurrentTargetInfo.LastKnownLocation = FVector::ZeroVector;
        CurrentTargetInfo.Distance = 0.0f;
        CurrentTargetInfo.bIsVisible = false;
        CurrentTargetInfo.bHasLineOfSight = false;
    }
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsObject(TargetKeyName, Target);
        
        if (Target)
        {
            AIData.Blackboard->SetValueAsVector(TargetLocationKeyName, Target->GetActorLocation());
        }
    }
    
    // Broadcast target change
    OnAITargetChanged.Broadcast(Target);
    
    // Update engagement state
    bIsEngaged = (Target != nullptr);
}

void ABlackRemasteredAIController::ClearTarget()
{
    SetTarget(nullptr);
}

bool ABlackRemasteredAIController::HasLineOfSightTo(AActor* Actor) const
{
    if (!Actor || !GetPawn())
    {
        return false;
    }
    
    // Trace from pawn to actor
    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(GetPawn());
    TraceParams.AddIgnoredActor(Actor);
    
    FVector StartLocation = GetPawn()->GetActorLocation();
    StartLocation.Z += 50.0f;
    
    FVector EndLocation = Actor->GetActorLocation();
    EndLocation.Z += 50.0f;
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        TraceParams
    );
    
    return !bHit || HitResult.GetActor() == Actor;
}

void ABlackRemasteredAIController::FindCover()
{
    if (!GetPawn())
    {
        return;
    }
    
    // Find best cover
    FindBestCover();
    
    if (bIsInCover)
    {
        // Update blackboard
        if (AIData.Blackboard)
        {
            AIData.Blackboard->SetValueAsVector(CoverLocationKeyName, CurrentCoverInfo.CoverLocation);
        }
    }
}

void ABlackRemasteredAIController::TakeCover()
{
    if (!bIsInCover || !GetPawn())
    {
        return;
    }
    
    // Move to cover location
    MoveToLocation(CurrentCoverInfo.CoverLocation);
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->SetValueAsEnum(BehaviorKeyName, static_cast<uint8>(EB_Cover));
    }
    
    // Broadcast cover change
    OnAICoverChanged.Broadcast(true);
}

void ABlackRemasteredAIController::ExitCover()
{
    if (!bIsInCover)
    {
        return;
    }
    
    bIsInCover = false;
    CurrentCoverInfo = FAICoverInfo();
    
    // Update blackboard
    if (AIData.Blackboard)
    {
        AIData.Blackboard->ClearValue(CoverLocationKeyName);
    }
    
    // Broadcast cover change
    OnAICoverChanged.Broadcast(false);
}

void ABlackRemasteredAIController::FindFlankPosition(FVector& OutPosition)
{
    FindBestFlankPosition(OutPosition);
}

void ABlackRemasteredAIController::FindRegroupPosition(FVector& OutPosition)
{
    FindBestRegroupPosition(OutPosition);
}

void ABlackRemasteredAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    // Update target based on perception
    for (AActor* Actor : UpdatedActors)
    {
        // Check if this is a player character
        if (ABlackRemasteredCharacter* Player = Cast<ABlackRemasteredCharacter>(Actor))
        {
            if (Player->IsAlive())
            {
                // Check if we can sense this player
                FActorPerceptionBlueprintInfo PerceptionInfo;
                GetAIPerceptionComponent()->GetActorsPerception(Actor, PerceptionInfo);
                
                for (FAIStimulus Stimulus : PerceptionInfo.LastSensedStimuli)
                {
                    if (Stimulus.WasSuccessfullySensed())
                    {
                        // Set target
                        SetTarget(Player);
                        
                        // Update enemy behavior
                        if (EnemyCharacter)
                        {
                            EnemyCharacter->DetectPlayer();
                        }
                        
                        return;
                    }
                }
            }
        }
    }
}

void ABlackRemasteredAIController::OnDamageTaken(float Damage, AController* Instigator, AActor* DamageCauser)
{
    if (!EnemyCharacter)
    {
        return;
    }
    
    // React to damage
    EnemyCharacter->SetEnemyState(ES_Damaged);
    
    // Check if we should retreat
    if (EnemyCharacter->GetHealthPercentage() <= AIData.RetreatHealthThreshold * 100.0f)
    {
        if (FMath::FRand() < 0.5f) // 50% chance to retreat when low health
        {
            StartRetreat();
        }
    }
    
    // Alert nearby enemies
    AlertNearbyEnemies();
}

void ABlackRemasteredAIController::OnDeath(AController* Killer)
{
    // Stop AI
    StopAI();
    
    // Clear target
    ClearTarget();
    
    // Exit cover
    ExitCover();
    
    // Stop movement
    StopMovement();
}

// Timer callbacks
void ABlackRemasteredAIController::OnDecisionTimer()
{
    // Make AI decision
    MakeDecision();
    ExecuteDecision();
    
    // Broadcast decision made
    OnAIDecisionMade.Broadcast();
}

void ABlackRemasteredAIController::OnPerceptionTimer()
{
    UpdatePerception();
}

void ABlackRemasteredAIController::OnAttackTimer()
{
    if (!EnemyCharacter || !CurrentTargetInfo.TargetActor || !bIsEngaged)
    {
        return;
    }
    
    // Check if we have line of sight
    if (HasLineOfSightTo(CurrentTargetInfo.TargetActor))
    {
        // Check if we should fire
        if (FMath::FRand() < AIData.Accuracy)
        {
            // Fire at target
            EnemyCharacter->Fire();
        }
        
        // Check if we should take cover
        if (FMath::FRand() < 0.3f) // 30% chance to take cover
        {
            MoveToCover();
        }
        
        // Check if we should flank
        if (FMath::FRand() < 0.2f) // 20% chance to flank
        {
            StartFlank();
        }
    }
    else
    {
        // Lost line of sight, search for target
        StartSearch();
    }
}

void ABlackRemasteredAIController::OnCoverTimer()
{
    // Cover timer expired
    ExitCover();
}

// Helper functions
void ABlackRemasteredAIController::UpdatePerception()
{
    if (!GetAIPerceptionComponent())
    {
        return;
    }
    
    // Get all perceived actors
    TArray<AActor*> PerceivedActors;
    GetAIPerceptionComponent()->GetCurrentlyPerceivedActors(UAISense::GetSenseImplementationClass<UAISense_Sight>(), PerceivedActors);
    
    // Find best target
    FindBestTarget();
}

void ABlackRemasteredAIController::UpdateTarget()
{
    if (!CurrentTargetInfo.TargetActor)
    {
        FindBestTarget();
        return;
    }
    
    // Check if target is still valid
    if (!CurrentTargetInfo.TargetActor->IsPendingKill() && 
        Cast<ABlackRemasteredCharacter>(CurrentTargetInfo.TargetActor)->IsAlive())
    {
        // Update target info
        CurrentTargetInfo.LastKnownLocation = CurrentTargetInfo.TargetActor->GetActorLocation();
        CurrentTargetInfo.Distance = FVector::Distance(GetPawn()->GetActorLocation(), CurrentTargetInfo.TargetActor->GetActorLocation());
        CurrentTargetInfo.bIsVisible = HasLineOfSightTo(CurrentTargetInfo.TargetActor);
        CurrentTargetInfo.bHasLineOfSight = CurrentTargetInfo.bIsVisible;
        CurrentTargetInfo.TimeSinceLastSeen += GetWorld()->GetDeltaSeconds();
        
        // Check if we've lost the target
        if (CurrentTargetInfo.TimeSinceLastSeen > AIData.MemoryTime)
        {
            ClearTarget();
        }
    }
    else
    {
        ClearTarget();
    }
}

void ABlackRemasteredAIController::UpdateBehavior()
{
    if (!EnemyCharacter)
    {
        return;
    }
    
    // Update behavior based on current state
    EEnemyBehavior CurrentBehavior = EnemyCharacter->GetEnemyBehavior();
    
    switch (CurrentBehavior)
    {
        case EB_Idle:
        case EB_Patrol:
            // Check for target
            if (HasTarget())
            {
                EnemyCharacter->SetEnemyBehavior(EB_Alert);
            }
            break;
            
        case EB_Alert:
            // Try to find target
            if (!HasTarget())
            {
                EnemyCharacter->SetEnemyBehavior(EB_Search);
            }
            else if (HasLineOfSightTo(CurrentTargetInfo.TargetActor))
            {
                EnemyCharacter->SetEnemyBehavior(EB_Engage);
            }
            break;
            
        case EB_Search:
            // Continue searching
            if (HasTarget() && HasLineOfSightTo(CurrentTargetInfo.TargetActor))
            {
                EnemyCharacter->SetEnemyBehavior(EB_Engage);
            }
            break;
            
        case EB_Investigate:
            // Continue investigating
            if (HasTarget() && HasLineOfSightTo(CurrentTargetInfo.TargetActor))
            {
                EnemyCharacter->SetEnemyBehavior(EB_Engage);
            }
            break;
            
        case EB_Engage:
            // Continue engaging
            if (!HasTarget())
            {
                EnemyCharacter->SetEnemyBehavior(EB_Search);
            }
            else if (!HasLineOfSightTo(CurrentTargetInfo.TargetActor))
            {
                EnemyCharacter->SetEnemyBehavior(EB_Search);
            }
            break;
            
        case EB_Suppress:
            // Continue suppressing
            if (!HasTarget())
            {
                EnemyCharacter->SetEnemyBehavior(EB_Search);
            }
            break;
            
        case EB_Flank:
            // Continue flanking
            if (!HasTarget())
            {
                EnemyCharacter->SetEnemyBehavior(EB_Search);
            }
            else if (HasLineOfSightTo(CurrentTargetInfo.TargetActor))
            {
                EnemyCharacter->SetEnemyBehavior(EB_Engage);
            }
            break;
            
        case EB_Retreat:
            // Continue retreating
            if (EnemyCharacter->GetHealthPercentage() > AIData.RetreatHealthThreshold * 100.0f * 1.5f)
            {
                // Health recovered, stop retreating
                bIsRetreating = false;
                EnemyCharacter->SetEnemyBehavior(EB_Engage);
            }
            break;
            
        case EB_Regroup:
            // Continue regrouping
            break;
            
        case EB_Cover:
            // Stay in cover
            if (HasTarget() && HasLineOfSightTo(CurrentTargetInfo.TargetActor))
            {
                // Fire from cover
                if (FMath::FRand() < AIData.Accuracy * 0.7f)
                {
                    EnemyCharacter->Fire();
                }
            }
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredAIController::MakeDecision()
{
    if (!EnemyCharacter || !EnemyCharacter->IsAlive())
    {
        return;
    }
    
    EEnemyBehavior CurrentBehavior = EnemyCharacter->GetEnemyBehavior();
    
    // Make decision based on current behavior and state
    switch (CurrentBehavior)
    {
        case EB_Idle:
            // Randomly start patrolling
            if (FMath::FRand() < 0.1f)
            {
                StartPatrol();
            }
            break;
            
        case EB_Patrol:
            // Check if we should stop patrolling
            if (FMath::FRand() < 0.05f)
            {
                StartPatrol(); // Will move to next point
            }
            break;
            
        case EB_Alert:
            // Check if we should start searching
            if (FMath::FRand() < 0.3f)
            {
                StartSearch();
            }
            break;
            
        case EB_Search:
            // Check if we should stop searching
            if (FMath::FRand() < 0.2f)
            {
                StartPatrol();
            }
            break;
            
        case EB_Engage:
            // Randomly switch to suppress or flank
            if (FMath::FRand() < 0.2f)
            {
                if (FMath::FRand() < 0.5f)
                {
                    StartSuppress();
                }
                else
                {
                    StartFlank();
                }
            }
            else if (FMath::FRand() < 0.1f)
            {
                // Take cover
                MoveToCover();
            }
            break;
            
        case EB_Suppress:
            // Check if we should stop suppressing
            if (FMath::FRand() < 0.3f)
            {
                StartEngage();
            }
            break;
            
        case EB_Flank:
            // Check if we should stop flanking
            if (FMath::FRand() < 0.5f)
            {
                StartEngage();
            }
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredAIController::ExecuteDecision()
{
    // Execute the decision made in MakeDecision()
    // This is handled by the behavior tree
}

void ABlackRemasteredAIController::FindBestTarget()
{
    if (!GetPawn())
    {
        return;
    }
    
    // Get all player characters
    TArray<AActor*> PlayerCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlackRemasteredCharacter::StaticClass(), PlayerCharacters);
    
    // Find best target
    AActor* BestTarget = nullptr;
    float BestScore = -1.0f;
    
    for (AActor* Actor : PlayerCharacters)
    {
        if (ABlackRemasteredCharacter* Player = Cast<ABlackRemasteredCharacter>(Actor))
        {
            if (Player->IsAlive())
            {
                // Evaluate target
                EvaluateTarget(Player);
                
                // Calculate score
                float Score = 0.0f;
                
                // Distance score (closer is better)
                float Distance = FVector::Distance(GetPawn()->GetActorLocation(), Player->GetActorLocation());
                Score += (AIData.SightRadius - Distance) / AIData.SightRadius;
                
                // Line of sight score
                if (HasLineOfSightTo(Player))
                {
                    Score += 0.5f;
                }
                
                // Health score (target with lower health is better)
                Score += (1.0f - Player->GetHealthComponent()->GetHealthPercentage() / 100.0f) * 0.2f;
                
                // If this target has a better score, select it
                if (Score > BestScore)
                {
                    BestScore = Score;
                    BestTarget = Player;
                }
            }
        }
    }
    
    // Set best target
    SetTarget(BestTarget);
}

void ABlackRemasteredAIController::EvaluateTarget(AActor* Actor)
{
    if (!Actor)
    {
        return;
    }
    
    // Update target evaluation
    // This could include threat level, distance, visibility, etc.
}

void ABlackRemasteredAIController::FindBestCover()
{
    if (!GetPawn())
    {
        bIsInCover = false;
        return;
    }
    
    // Trace in multiple directions for cover
    FVector PawnLocation = GetPawn()->GetActorLocation();
    
    // Check in 8 directions
    TArray<FVector> Directions = {
        FVector(1, 0, 0),
        FVector(1, 1, 0).GetSafeNormal(),
        FVector(0, 1, 0),
        FVector(-1, 1, 0).GetSafeNormal(),
        FVector(-1, 0, 0),
        FVector(-1, -1, 0).GetSafeNormal(),
        FVector(0, -1, 0),
        FVector(1, -1, 0).GetSafeNormal()
    };
    
    FAICoverInfo BestCover;
    float BestScore = -1.0f;
    
    for (FVector Direction : Directions)
    {
        // Trace for cover in this direction
        FVector TraceStart = PawnLocation;
        TraceStart.Z += 50.0f;
        
        FVector TraceEnd = TraceStart + (Direction * AIData.CoverSearchRadius);
        
        FHitResult HitResult;
        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(GetPawn());
        
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            ECC_Visibility,
            TraceParams
        );
        
        if (bHit)
        {
            // Check if this is valid cover
            if (IsValidCover(HitResult.ImpactPoint, HitResult.Normal))
            {
                // Calculate score
                float Score = 0.0f;
                
                // Distance score (closer is better)
                float Distance = FVector::Distance(PawnLocation, HitResult.ImpactPoint);
                Score += (AIData.CoverSearchRadius - Distance) / AIData.CoverSearchRadius;
                
                // Height score (taller is better)
                // Trace down to find height
                FVector HeightTraceStart = HitResult.ImpactPoint;
                HeightTraceStart.Z += 100.0f;
                
                FVector HeightTraceEnd = HeightTraceStart;
                HeightTraceEnd.Z -= 300.0f;
                
                FHitResult HeightHit;
                bool bHeightHit = GetWorld()->LineTraceSingleByChannel(
                    HeightHit,
                    HeightTraceStart,
                    HeightTraceEnd,
                    ECC_Visibility,
                    TraceParams
                );
                
                float Height = 0.0f;
                if (bHeightHit)
                {
                    Height = FVector::Distance(HeightTraceStart, HeightHit.ImpactPoint);
                }
                else
                {
                    Height = FVector::Distance(HeightTraceStart, HeightTraceEnd);
                }
                
                Score += FMath::Min(Height / 300.0f, 1.0f) * 0.5f;
                
                // If this cover has a better score, select it
                if (Score > BestScore)
                {
                    BestScore = Score;
                    BestCover.CoverLocation = HitResult.ImpactPoint;
                    BestCover.CoverNormal = HitResult.Normal;
                    BestCover.CoverHeight = Height;
                    BestCover.bCanLeanLeft = true;
                    BestCover.bCanLeanRight = true;
                }
            }
        }
    }
    
    // Set best cover
    if (BestScore > -0.5f)
    {
        CurrentCoverInfo = BestCover;
        bIsInCover = true;
    }
    else
    {
        bIsInCover = false;
    }
}

bool ABlackRemasteredAIController::IsValidCover(const FVector& Location, const FVector& Normal) const
{
    // Check if the surface is vertical enough
    float SurfaceAngle = FMath::Abs(FMath::Asin(Normal.Z));
    
    if (SurfaceAngle < 60.0f) // More than 60 degrees from horizontal (mostly vertical)
    {
        return false;
    }
    
    // Check if the cover is tall enough
    // We'll check this in FindBestCover()
    
    return true;
}

void ABlackRemasteredAIController::FindBestFlankPosition(FVector& OutPosition)
{
    if (!GetPawn() || !CurrentTargetInfo.TargetActor)
    {
        OutPosition = GetPawn()->GetActorLocation();
        return;
    }
    
    // Calculate position to flank target
    FVector PawnLocation = GetPawn()->GetActorLocation();
    FVector TargetLocation = CurrentTargetInfo.TargetActor->GetActorLocation();
    
    // Calculate direction to target
    FVector DirectionToTarget = (TargetLocation - PawnLocation).GetSafeNormal();
    
    // Calculate flank direction (perpendicular to target direction)
    FVector FlankDirection = FVector::CrossProduct(DirectionToTarget, FVector::UpVector).GetSafeNormal();
    
    // Randomly choose left or right flank
    if (FMath::FRand() < 0.5f)
    {
        FlankDirection = -FlankDirection;
    }
    
    // Calculate flank position
    OutPosition = PawnLocation + (FlankDirection * AIData.FlankDistance);
    
    // Add some randomness
    OutPosition.X += FMath::FRandRange(-50.0f, 50.0f);
    OutPosition.Y += FMath::FRandRange(-50.0f, 50.0f);
    
    // Try to find a valid navigation point
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSystem)
    {
        FNavLocation NavLocation;
        if (NavSystem->ProjectPointToNavigation(OutPosition, NavLocation, AIData.FlankDistance))
        {
            OutPosition = NavLocation.Location;
        }
    }
}

void ABlackRemasteredAIController::FindBestRegroupPosition(FVector& OutPosition)
{
    if (!GetPawn())
    {
        OutPosition = GetPawn()->GetActorLocation();
        return;
    }
    
    // Find other enemies of the same type
    TArray<AActor*> EnemyCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlackRemasteredEnemyCharacter::StaticClass(), EnemyCharacters);
    
    // Find closest ally
    AActor* ClosestAlly = nullptr;
    float MinDistance = TNumericLimits<float>::Max();
    
    for (AActor* Actor : EnemyCharacters)
    {
        if (ABlackRemasteredEnemyCharacter* Enemy = Cast<ABlackRemasteredEnemyCharacter>(Actor))
        {
            if (Enemy != EnemyCharacter && Enemy->IsAlive())
            {
                float Distance = FVector::Distance(GetPawn()->GetActorLocation(), Enemy->GetActorLocation());
                
                if (Distance < MinDistance)
                {
                    MinDistance = Distance;
                    ClosestAlly = Enemy;
                }
            }
        }
    }
    
    if (ClosestAlly)
    {
        // Regroup near closest ally
        OutPosition = ClosestAlly->GetActorLocation();
        
        // Add some offset
        FVector Offset = FVector(FMath::FRandRange(-100.0f, 100.0f), FMath::FRandRange(-100.0f, 100.0f), 0.0f);
        OutPosition += Offset;
    }
    else
    {
        // No allies, regroup at a random location
        FVector RandomDirection = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
        OutPosition = GetPawn()->GetActorLocation() + (RandomDirection * 300.0f);
    }
}

void ABlackRemasteredAIController::StopAI()
{
    // Stop movement
    StopMovement();
    
    // Clear timers
    GetWorld()->GetTimerManager().ClearTimer(DecisionTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(PerceptionTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(CoverTimerHandle);
    
    // Clear target
    ClearTarget();
    
    // Exit cover
    ExitCover();
    
    // Reset state
    bIsEngaged = false;
    bIsFlanking = false;
    bIsRetreating = false;
}

void ABlackRemasteredAIController::CheckForObstacles()
{
    if (!GetPawn() || !GetPathFollowingComponent())
    {
        return;
    }
    
    // Check if we're stuck
    if (GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Blocked)
    {
        AvoidObstacle();
    }
}

void ABlackRemasteredAIController::AvoidObstacle()
{
    if (!GetPawn())
    {
        return;
    }
    
    // Try to find a new path
    if (CurrentTargetInfo.TargetActor)
    {
        MoveToActor(CurrentTargetInfo.TargetActor);
    }
    else if (!CurrentCoverInfo.CoverLocation.IsNearlyZero())
    {
        MoveToLocation(CurrentCoverInfo.CoverLocation);
    }
}

void ABlackRemasteredAIController::AlertNearbyEnemies()
{
    if (!EnemyCharacter)
    {
        return;
    }
    
    // Find nearby enemies
    TArray<AActor*> EnemyCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlackRemasteredEnemyCharacter::StaticClass(), EnemyCharacters);
    
    // Alert enemies within a radius
    float AlertRadius = 1000.0f;
    
    for (AActor* Actor : EnemyCharacters)
    {
        if (ABlackRemasteredEnemyCharacter* Enemy = Cast<ABlackRemasteredEnemyCharacter>(Actor))
        {
            if (Enemy != EnemyCharacter && Enemy->IsAlive())
            {
                float Distance = FVector::Distance(EnemyCharacter->GetActorLocation(), Enemy->GetActorLocation());
                
                if (Distance <= AlertRadius)
                {
                    // Alert this enemy
                    if (ABlackRemasteredAIController* EnemyAI = Enemy->GetEnemyAIController())
                    {
                        EnemyAI->SetTarget(CurrentTargetInfo.TargetActor);
                        Enemy->SetEnemyBehavior(EB_Alert);
                    }
                }
            }
        }
    }
}

void ABlackRemasteredAIController::RequestBackup()
{
    // Request backup from other enemies
    AlertNearbyEnemies();
}

void ABlackRemasteredAIController::RequestFlank()
{
    // Request flank from other enemies
    // Similar to AlertNearbyEnemies but specifically for flanking
}

void ABlackRemasteredAIController::SetupPatrolPoints()
{
    // Set up patrol points for this enemy
    // This would be specific to each level
}

void ABlackRemasteredAIController::MoveToNextPatrolPoint()
{
    // Move to the next patrol point
    // This would cycle through predefined patrol points
}

void ABlackRemasteredAIController::SearchArea(const FVector& Center, float Radius)
{
    if (!GetPawn())
    {
        return;
    }
    
    // Find a random point within the search area
    FVector RandomOffset = FVector(
        FMath::FRandRange(-Radius, Radius),
        FMath::FRandRange(-Radius, Radius),
        0.0f
    );
    
    FVector SearchLocation = Center + RandomOffset;
    
    // Try to find a valid navigation point
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSystem)
    {
        FNavLocation NavLocation;
        if (NavSystem->ProjectPointToNavigation(SearchLocation, NavLocation, Radius))
        {
            SearchLocation = NavLocation.Location;
        }
    }
    
    // Move to search location
    MoveToLocation(SearchLocation);
}

void ABlackRemasteredAIController::SuppressArea(const FVector& Location)
{
    if (!EnemyCharacter || !EnemyCharacter->IsAlive())
    {
        return;
    }
    
    // Face towards location
    FVector Direction = (Location - GetPawn()->GetActorLocation()).GetSafeNormal();
    FRotator NewRotation = Direction.Rotation();
    GetPawn()->SetActorRotation(NewRotation);
    
    // Fire at location
    EnemyCharacter->Fire();
}

void ABlackRemasteredAIController::ThrowGrenadeAtLocation(const FVector& Location)
{
    if (!EnemyCharacter || !EnemyCharacter->IsAlive())
    {
        return;
    }
    
    // Check if we can use grenades
    if (EnemyCharacter->GetEnemyData().bCanUseGrenades)
    {
        // Face towards location
        FVector Direction = (Location - GetPawn()->GetActorLocation()).GetSafeNormal();
        FRotator NewRotation = Direction.Rotation();
        GetPawn()->SetActorRotation(NewRotation);
        
        // Throw grenade
        EnemyCharacter->ThrowGrenade();
    }
}
