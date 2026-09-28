#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackRemastered.h"
#include "BlackRemasteredCoverComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoverEntered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoverExited);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoverHeightChanged, float, Height);

class ABlackRemasteredCharacter;
class AActor;

UENUM(BlueprintType)
enum class ECoverSide : uint8
{
    CS_None UMETA(DisplayName = "None"),
    CS_Left UMETA(DisplayName = "Left"),
    CS_Right UMETA(DisplayName = "Right"),
    CS_Front UMETA(DisplayName = "Front"),
    CS_Back UMETA(DisplayName = "Back"),
    CS_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECoverType : uint8
{
    CT_None UMETA(DisplayName = "None"),
    CT_Low UMETA(DisplayName = "Low Cover"),
    CT_High UMETA(DisplayName = "High Cover"),
    CT_Full UMETA(DisplayName = "Full Cover"),
    CT_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FCoverData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    AActor* CoverActor;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    FVector CoverLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    FVector CoverNormal;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    ECoverSide CoverSide;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    ECoverType CoverType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    float CoverHeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    float CoverWidth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    bool bCanLeanLeft;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    bool bCanLeanRight;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKREMASTERED_API UBlackRemasteredCoverComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBlackRemasteredCoverComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void Initialize(ABlackRemasteredCharacter* OwnerCharacter);
    
    // Cover management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void CheckForCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void UpdateCoverState();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void EnterCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void ExitCover();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void ToggleCover();
    
    // Lean
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void StartLeanLeft();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void StartLeanRight();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    void StopLean();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    bool CanLean() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    bool CanLeanLeft() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Cover")
    bool CanLeanRight() const;
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Cover")
    bool IsInCover() const { return bIsInCover; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Cover")
    ECoverSide GetCoverSide() const { return CurrentCoverSide; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Cover")
    ECoverType GetCoverType() const { return CurrentCoverType; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Cover")
    float GetCoverHeight() const { return CurrentCoverHeight; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Cover")
    AActor* GetCoverActor() const { return CurrentCoverData.CoverActor; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Cover")
    FCoverData GetCurrentCoverData() const { return CurrentCoverData; }
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Cover")
    FOnCoverEntered OnCoverEntered;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Cover")
    FOnCoverExited OnCoverExited;
    
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Cover")
    FOnCoverHeightChanged OnCoverHeightChanged;
    
protected:
    // Owner
    UPROPERTY()
    ABlackRemasteredCharacter* OwnerCharacter;
    
    // Cover state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Cover")
    bool bIsInCover;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Cover")
    ECoverSide CurrentCoverSide;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Cover")
    ECoverType CurrentCoverType;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Cover")
    float CurrentCoverHeight;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Cover")
    FCoverData CurrentCoverData;
    
    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Cover")
    float CoverDetectionRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Cover")
    float CoverDetectionAngle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Cover")
    float CoverSnapDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Cover")
    float CoverHeightThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Cover")
    float CoverWidthThreshold;
    
    // Timers
    UPROPERTY()
    FTimerHandle CoverCheckTimerHandle;
    
    // Helper functions
    void OnCoverCheckTimer();
    
    bool FindCover(FCoverData& OutCoverData) const;
    bool IsValidCover(AActor* Actor, const FHitResult& HitResult) const;
    
    void ApplyCoverPosition();
    void ApplyCoverRotation();
    
    void UpdateCoverType();
    
    bool TraceForCover(FVector& Start, FVector& End, FHitResult& HitResult) const;
};
