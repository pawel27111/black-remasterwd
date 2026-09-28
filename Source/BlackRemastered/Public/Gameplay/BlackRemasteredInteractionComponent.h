#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackRemastered.h"
#include "BlackRemasteredInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFound, AActor*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableLost, AActor*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionStarted, AActor*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionCompleted, AActor*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionCancelled, AActor*, Interactable);

class ABlackRemasteredCharacter;
class AActor;

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
    IT_Pickup UMETA(DisplayName = "Pickup"),
    IT_Use UMETA(DisplayName = "Use"),
    IT_Open UMETA(DisplayName = "Open"),
    IT_Close UMETA(DisplayName = "Close"),
    IT_Activate UMETA(DisplayName = "Activate"),
    IT_Deactivate UMETA(DisplayName = "Deactivate"),
    IT_Read UMETA(DisplayName = "Read"),
    IT_Talk UMETA(DisplayName = "Talk"),
    IT_Climb UMETA(DisplayName = "Climb"),
    IT_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EInteractionState : uint8
{
    IS_None UMETA(DisplayName = "None"),
    IS_Found UMETA(DisplayName = "Found"),
    IS_Interacting UMETA(DisplayName = "Interacting"),
    IS_Cooldown UMETA(DisplayName = "Cooldown"),
    IS_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FInteractionData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FName InteractionName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    EInteractionType InteractionType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float CooldownDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bRequireLineOfSight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bCanCancel;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText DisplayText;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FName ActionName;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKREMASTERED_API UBlackRemasteredInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBlackRemasteredInteractionComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Interaction")
    void Initialize(ABlackRemasteredCharacter* OwnerCharacter);
    
    // Interaction
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Interaction")
    void Interact();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Interaction")
    void InteractWithActor(AActor* Actor);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Interaction")
    void CancelInteraction();
    
    // Detection
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Interaction")
    void DetectInteractables();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Interaction")
    void ClearInteractables();
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Interaction")
    AActor* GetCurrentInteractable() const { return CurrentInteractable; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Interaction")
    TArray<AActor*> GetAllInteractables() const { return Interactables; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Interaction")
    EInteractionState GetInteractionState() const { return CurrentInteractionState; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Interaction")
    FText GetInteractionDisplayText() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Interaction")
    float GetInteractionProgress() const { return InteractionProgress; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Interaction")
    bool CanInteract() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Interaction")
    bool HasInteractables() const { return Interactables.Num() > 0; }
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Interaction")
    FOnInteractableFound OnInteractableFound;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Interaction")
    FOnInteractableLost OnInteractableLost;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Interaction")
    FOnInteractionStarted OnInteractionStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Interaction")
    FOnInteractionCompleted OnInteractionCompleted;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Interaction")
    FOnInteractionCancelled OnInteractionCancelled;
    
protected:
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // Interactables
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Interaction")
    TArray<AActor*> Interactables;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Interaction")
    AActor* CurrentInteractable;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Interaction")
    EInteractionState CurrentInteractionState;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Interaction")
    float InteractionProgress;
    
    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Interaction")
    float DetectionRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Interaction")
    float DetectionAngle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Interaction")
    float DetectionFrequency;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Interaction")
    bool bRequireLineOfSight;
    
    // Timers
    UPROPERTY()
    FTimerHandle DetectionTimerHandle;
    
    UPROPERTY()
    FTimerHandle InteractionTimerHandle;
    
    UPROPERTY()
    FTimerHandle CooldownTimerHandle;
    
    // Helper functions
    void OnDetectionTimer();
    void OnInteractionTimer();
    void OnCooldownTimer();
    
    void FindInteractables();
    void SortInteractables();
    void SelectBestInteractable();
    
    bool IsValidInteractable(AActor* Actor) const;
    bool HasLineOfSight(AActor* Actor) const;
    
    void StartInteraction(AActor* Actor);
    void CompleteInteraction();
    
    FInteractionData GetInteractionData(AActor* Actor) const;
};
