#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlackRemastered.h"
#include "BlackRemasteredGameMode.generated.h"

class ABlackRemasteredPlayerController;
class ABlackRemasteredCharacter;
class ABlackRemasteredAIController;
class UBlackRemasteredSaveSystem;
class UBlackRemasteredProgressionSystem;

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABlackRemasteredGameMode();
    
    virtual void StartPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;
    
    // Game state management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Game")
    void SetGameState(EGameState NewState);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Game")
    EGameState GetGameState() const { return CurrentGameState; }
    
    // Difficulty
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Game")
    void SetDifficulty(EGameDifficulty Difficulty);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Game")
    EGameDifficulty GetDifficulty() const { return CurrentDifficulty; }
    
    // Player management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Player")
    ABlackRemasteredCharacter* GetBlackRemasteredPlayer() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Player")
    ABlackRemasteredPlayerController* GetBlackRemasteredPlayerController() const;
    
    // Systems
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Systems")
    UBlackRemasteredSaveSystem* SaveSystem;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Systems")
    UBlackRemasteredProgressionSystem* ProgressionSystem;
    
    // Level management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Level")
    void LoadLevel(const FString& LevelName);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Level")
    void RestartLevel();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Level")
    void LoadNextLevel();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Level")
    void LoadPreviousLevel();
    
    // Game rules
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Rules")
    bool bPermadeathEnabled;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Rules")
    bool bIronmanMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Rules")
    int32 MaxLives;
    
    // Mission tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Mission")
    FString CurrentMissionName;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Mission")
    int32 CurrentMissionIndex;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChanged, EGameState, OldState, EGameState, NewState);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnGameStateChanged OnGameStateChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMissionStarted);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnMissionStarted OnMissionStarted;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMissionCompleted);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnMissionCompleted OnMissionCompleted;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameCompleted);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|Events")
    FOnGameCompleted OnGameCompleted;
    
protected:
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;
    
    // Game state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Game")
    EGameState CurrentGameState;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Game")
    EGameDifficulty CurrentDifficulty;
    
    // Level tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Level")
    FString CurrentLevelName;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Level")
    int32 CurrentLevelIndex;
    
    // Mission data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Mission")
    TArray<FString> MissionNames;
    
    // Timers
    UPROPERTY()
    FTimerHandle GameTimerHandle;
    
    void OnGameTimer();
    void ApplyDifficultySettings();
    
public:
    // Difficulty modifiers
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Difficulty")
    float GetPlayerDamageMultiplier() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Difficulty")
    float GetEnemyDamageMultiplier() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Difficulty")
    float GetEnemyHealthMultiplier() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Difficulty")
    float GetEnemyAccuracyMultiplier() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Difficulty")
    float GetAmmoScarcityMultiplier() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Difficulty")
    float GetEnemySpawnRateMultiplier() const;
    
    // Mission management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Mission")
    void StartMission(int32 MissionIndex);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Mission")
    void CompleteMission();
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Mission")
    int32 GetMissionCount() const { return MissionNames.Num(); }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Mission")
    FString GetCurrentMissionName() const { return CurrentMissionName; }
    
    // Game completion
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Game")
    void CompleteGame();
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Game")
    bool IsGameCompleted() const;
};
