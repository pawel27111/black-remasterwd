#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BlackRemastered.h"
#include "BlackRemasteredAIController.generated.h"

class ABlackRemasteredEnemyCharacter;
class UBehaviorTree;
class UBlackboardComponent;
class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAITask : uint8
{
    AIT_None UMETA(DisplayName = "None"),
    AIT_MoveTo UMETA(DisplayName = "Move To"),
    AIT_Attack UMETA(DisplayName = "Attack"),
    AIT_TakeCover UMETA(DisplayName = "Take Cover"),
    AIT_Flank UMETA(DisplayName = "Flank"),
    AIT_Retreat UMETA(DisplayName = "Retreat"),
    AIT_Regroup UMETA(DisplayName = "Regroup"),
    AIT_UseGrenade UMETA(DisplayName = "Use Grenade"),
    AIT_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FAIData
{
    GENERATED_BODY()
    
    // Behavior tree
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior")
    UBehaviorTree* BehaviorTree;
    
    // Blackboard
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior")
    UBlackboardComponent* Blackboard;
    
    // Perception
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Perception")
    UPawnSensingComponent* PawnSensing;
    
    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Settings")
    float SightRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Settings")
    float SightAngle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Settings")
    float HearingRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Settings")
    float MemoryTime;
    
    // Movement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
    float MoveSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
    float RotationSpeed;
    
    // Combat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    float AttackRange;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    float AttackCooldown;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    float Accuracy;
    
    // Cover
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Cover")
    float CoverSearchRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Cover")
    float CoverMinHeight;
    
    // Flanking
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Flanking")
    float FlankDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Flanking")
    float FlankAngle;
    
    // Retreat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Retreat")
    float RetreatDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Retreat")
    float RetreatHealthThreshold;
};

USTRUCT(BlueprintType)
struct FAITargetInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Target")
    AActor* TargetActor;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Target")
    FVector LastKnownLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Target")
    float Distance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Target")
    float TimeSinceLastSeen;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Target")
    bool bHasLineOfSight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Target")
    bool bIsVisible;
};

USTRUCT(BlueprintType)
struct FAICoverInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Cover")
    FVector CoverLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Cover")
    FVector CoverNormal;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Cover")
    float CoverHeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Cover")
    bool bCanLeanLeft;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Cover")
    bool bCanLeanRight;
};

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABlackRemasteredAIController();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Possess(APawn* InPawn) override;
    virtual void UnPossess() override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void InitializeEnemy(ABlackRemasteredEnemyCharacter* Enemy);
    
    // Movement
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void MoveToLocation(const FVector& Location);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void MoveToActor(AActor* Actor);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void StopMovement();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void SetMoveSpeed(float Speed);
    
    // Behavior
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void StartPatrol();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void StartSearch();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void StartEngage();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void StartSuppress();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void StartFlank();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void StartRetreat();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void StartRegroup();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void MoveToCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void UseGrenade();
    
    // Target
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void SetTarget(AActor* Target);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void ClearTarget();
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|AI")
    AActor* GetTarget() const { return CurrentTargetInfo.TargetActor; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|AI")
    bool HasTarget() const { return CurrentTargetInfo.TargetActor != nullptr; }
    
    // Cover
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void FindCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void TakeCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void ExitCover();
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|AI")
    bool IsInCover() const { return bIsInCover; }
    
    // Flanking
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void FindFlankPosition(FVector& OutPosition);
    
    // Regroup
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void FindRegroupPosition(FVector& OutPosition);
    
    // Perception
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
    
    // Damage
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void OnDamageTaken(float Damage, AController* Instigator, AActor* DamageCauser);
    
    // Death
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|AI")
    void OnDeath(AController* Killer);
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|AI")
    ABlackRemasteredEnemyCharacter* GetEnemyCharacter() const { return EnemyCharacter; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|AI")
    FAIData GetAIData() const { return AIData; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|AI")
    FAITargetInfo GetTargetInfo() const { return CurrentTargetInfo; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|AI")
    FAICoverInfo GetCoverInfo() const { return CurrentCoverInfo; }
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAITargetChanged, AActor*, NewTarget);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|AI")
    FOnAITargetChanged OnAITargetChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAICoverChanged, bool, bInCover);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|AI")
    FOnAICoverChanged OnAICoverChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIDecisionMade);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|AI")
    FOnAIDecisionMade OnAIDecisionMade;
    
protected:
    // Enemy character
    UPROPERTY()
    ABlackRemasteredEnemyCharacter* EnemyCharacter;
    
    // AI data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|AI")
    FAIData AIData;
    
    // Target info
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|AI")
    FAITargetInfo CurrentTargetInfo;
    
    // Cover info
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|AI")
    FAICoverInfo CurrentCoverInfo;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|AI")
    bool bIsInCover;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|AI")
    bool bIsEngaged;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|AI")
    bool bIsFlanking;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|AI")
    bool bIsRetreating;
    
    // Timers
    UPROPERTY()
    FTimerHandle DecisionTimerHandle;
    
    UPROPERTY()
    FTimerHandle PerceptionTimerHandle;
    
    UPROPERTY()
    FTimerHandle AttackTimerHandle;
    
    UPROPERTY()
    FTimerHandle CoverTimerHandle;
    
    // Helper functions
    void OnDecisionTimer();
    void OnPerceptionTimer();
    void OnAttackTimer();
    void OnCoverTimer();
    
    void UpdatePerception();
    void UpdateTarget();
    void UpdateBehavior();
    
    void MakeDecision();
    void ExecuteDecision();
    
    void FindBestTarget();
    void EvaluateTarget(AActor* Actor);
    
    void FindBestCover();
    bool IsValidCover(const FVector& Location, const FVector& Normal) const;
    
    void FindBestFlankPosition(FVector& OutPosition);
    void FindBestRegroupPosition(FVector& OutPosition);
    
    void StopAI();
    
    // Navigation
    void CheckForObstacles();
    void AvoidObstacle();
    
    // Communication
    void AlertNearbyEnemies();
    void RequestBackup();
    void RequestFlank();
    
    // Patrol
    void SetupPatrolPoints();
    void MoveToNextPatrolPoint();
    
    // Search
    void SearchArea(const FVector& Center, float Radius);
    
    // Suppression
    void SuppressArea(const FVector& Location);
    
    // Grenade
    void ThrowGrenadeAtLocation(const FVector& Location);
    
    // Blackboard keys
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|AI")
    FName TargetKeyName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|AI")
    FName TargetLocationKeyName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|AI")
    FName CoverLocationKeyName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|AI")
    FName BehaviorKeyName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|AI")
    FName StateKeyName;
};
