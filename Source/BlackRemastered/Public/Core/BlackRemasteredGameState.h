#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BlackRemastered.h"
#include "BlackRemasteredGameState.generated.h"

class ABlackRemasteredCharacter;

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ABlackRemasteredGameState();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Level tracking
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Level")
    FString CurrentLevelName;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Level")
    int32 CurrentLevelIndex;
    
    // Mission tracking
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Mission")
    FString CurrentMissionName;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Mission")
    int32 CurrentMissionIndex;
    
    // Statistics
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    int32 TotalKills;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    int32 TotalDeaths;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    int32 TotalShotsFired;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    int32 TotalShotsHit;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    int32 TotalHeadshots;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    int32 TotalDestruction;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    float TotalPlayTime;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    int32 TotalExplosions;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    int32 TotalGrenadesThrown;
    
    // Accuracy calculation
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Stats")
    float GetAccuracy() const;
    
    // K/D ratio calculation
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Stats")
    float GetKDRatio() const;
    
    // Headshot percentage calculation
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Stats")
    float GetHeadshotPercentage() const;
    
    // Add kill
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddKill();
    
    // Add death
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddDeath();
    
    // Add shot
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddShot(bool bHit, bool bHeadshot = false);
    
    // Add destruction
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddDestruction(int32 Amount);
    
    // Add play time
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddPlayTime(float DeltaTime);
    
    // Add explosion
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddExplosion();
    
    // Add grenade
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddGrenade();
    
    // Reset statistics
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void ResetStatistics();
    
    // Level completion
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Level")
    bool bLevelCompleted;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Level")
    void SetLevelCompleted(bool bCompleted);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Level")
    bool IsLevelCompleted() const { return bLevelCompleted; }
    
    // Mission completion
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Mission")
    bool bMissionCompleted;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Mission")
    void SetMissionCompleted(bool bCompleted);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Mission")
    bool IsMissionCompleted() const { return bMissionCompleted; }
    
    // Game completion
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Game")
    bool bGameCompleted;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Game")
    void SetGameCompleted(bool bCompleted);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Game")
    bool IsGameCompleted() const { return bGameCompleted; }
    
protected:
    // Timer for play time tracking
    UPROPERTY()
    FTimerHandle PlayTimeTimerHandle;
    
    void OnPlayTimeTimer();
};
