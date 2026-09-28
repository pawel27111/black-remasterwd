#include "BlackRemasteredGameMode.h"
#include "BlackRemasteredCharacter.h"
#include "BlackRemasteredPlayerController.h"
#include "BlackRemasteredAIController.h"
#include "Save/BlackRemasteredSaveSystem.h"
#include "Progression/BlackRemasteredProgressionSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ABlackRemasteredGameMode::ABlackRemasteredGameMode()
    : Super()
{
    // Default pawn class
    DefaultPawnClass = ABlackRemasteredCharacter::StaticClass();
    
    // Default player controller class
    PlayerControllerClass = ABlackRemasteredPlayerController::StaticClass();
    
    // Default AI controller class
    AIControllerClass = ABlackRemasteredAIController::StaticClass();
    
    // Default difficulty
    CurrentDifficulty = GD_Veteran;
    CurrentGameState = GS_Playing;
    
    // Default game rules
    bPermadeathEnabled = false;
    bIronmanMode = false;
    MaxLives = 1;
    CurrentLevelIndex = 0;
    CurrentMissionIndex = 0;
    
    // Initialize mission names
    MissionNames = {
        "L_BlackDawn",
        "L_ScorchedEarth",
        "L_BridgeOfSighs",
        "L_GhostTown",
        "L_FactoryOfDeath",
        "L_TunnelVision",
        "L_MountainFortress",
        "L_HeartOfDarkness"
    };
}

void ABlackRemasteredGameMode::StartPlay()
{
    Super::StartPlay();
    
    // Initialize systems
    if (!SaveSystem)
    {
        SaveSystem = NewObject<UBlackRemasteredSaveSystem>(this);
    }
    
    if (!ProgressionSystem)
    {
        ProgressionSystem = NewObject<UBlackRemasteredProgressionSystem>(this);
    }
    
    // Load save data
    if (SaveSystem)
    {
        SaveSystem->LoadGame();
    }
    
    // Apply difficulty settings
    ApplyDifficultySettings();
}

void ABlackRemasteredGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Start game timer
    GetWorld()->GetTimerManager().SetTimer(
        GameTimerHandle, 
        this, 
        &ABlackRemasteredGameMode::OnGameTimer, 
        1.0f, 
        true
    );
}

void ABlackRemasteredGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Handle game state updates
    switch (CurrentGameState)
    {
        case GS_GameOver:
            // Handle game over logic
            break;
            
        case GS_Victory:
            // Handle victory logic
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    // Initialize player
    if (ABlackRemasteredPlayerController* BRPlayerController = Cast<ABlackRemasteredPlayerController>(NewPlayer))
    {
        BRPlayerController->InitializePlayer();
    }
}

void ABlackRemasteredGameMode::Logout(AController* Exiting)
{
    // Save game on logout
    if (SaveSystem)
    {
        SaveSystem->SaveGame();
    }
    
    Super::Logout(Exiting);
}

void ABlackRemasteredGameMode::SetGameState(EGameState NewState)
{
    EGameState OldState = CurrentGameState;
    CurrentGameState = NewState;
    
    // Broadcast state change
    OnGameStateChanged.Broadcast(OldState, NewState);
    
    // Handle state-specific logic
    switch (NewState)
    {
        case GS_Paused:
            // Pause game
            UGameplayStatics::SetGamePaused(GetWorld(), true);
            break;
            
        case GS_Playing:
            // Resume game
            UGameplayStatics::SetGamePaused(GetWorld(), false);
            break;
            
        case GS_GameOver:
            // Handle game over
            break;
            
        case GS_Victory:
            // Handle victory
            break;
            
        default:
            break;
    }
}

void ABlackRemasteredGameMode::SetDifficulty(EGameDifficulty Difficulty)
{
    CurrentDifficulty = Difficulty;
    ApplyDifficultySettings();
    
    // Save difficulty setting
    if (SaveSystem)
    {
        SaveSystem->SaveSettings();
    }
}

void ABlackRemasteredGameMode::ApplyDifficultySettings()
{
    // Apply difficulty-specific settings
    switch (CurrentDifficulty)
    {
        case GD_Recruit:
            bPermadeathEnabled = false;
            bIronmanMode = false;
            MaxLives = 3;
            break;
            
        case GD_Veteran:
            bPermadeathEnabled = false;
            bIronmanMode = false;
            MaxLives = 1;
            break;
            
        case GD_BlackOps:
            bPermadeathEnabled = false;
            bIronmanMode = false;
            MaxLives = 1;
            break;
            
        case GD_Hardcore:
            bPermadeathEnabled = true;
            bIronmanMode = true;
            MaxLives = 1;
            break;
            
        default:
            break;
    }
}

ABlackRemasteredCharacter* ABlackRemasteredGameMode::GetBlackRemasteredPlayer() const
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            if (APawn* Pawn = PlayerController->GetPawn())
            {
                return Cast<ABlackRemasteredCharacter>(Pawn);
            }
        }
    }
    return nullptr;
}

ABlackRemasteredPlayerController* ABlackRemasteredGameMode::GetBlackRemasteredPlayerController() const
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            return Cast<ABlackRemasteredPlayerController>(PlayerController);
        }
    }
    return nullptr;
}

void ABlackRemasteredGameMode::LoadLevel(const FString& LevelName)
{
    CurrentLevelName = LevelName;
    UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void ABlackRemasteredGameMode::RestartLevel()
{
    if (!CurrentLevelName.IsEmpty())
    {
        UGameplayStatics::OpenLevel(GetWorld(), CurrentLevelName);
    }
}

void ABlackRemasteredGameMode::LoadNextLevel()
{
    // Increment level index and load next level
    CurrentLevelIndex++;
    
    if (CurrentLevelIndex < MissionNames.Num())
    {
        LoadLevel(MissionNames[CurrentLevelIndex]);
    }
    else
    {
        // All levels completed
        CompleteGame();
    }
}

void ABlackRemasteredGameMode::LoadPreviousLevel()
{
    // Decrement level index and load previous level
    CurrentLevelIndex = FMath::Max(CurrentLevelIndex - 1, 0);
    LoadLevel(MissionNames[CurrentLevelIndex]);
}

void ABlackRemasteredGameMode::OnGameTimer()
{
    // Game timer update logic
    if (CurrentGameState == GS_Playing)
    {
        // Update game systems
    }
}

float ABlackRemasteredGameMode::GetPlayerDamageMultiplier() const
{
    switch (CurrentDifficulty)
    {
        case GD_Recruit: return 0.75f;
        case GD_Veteran: return 1.0f;
        case GD_BlackOps: return 1.25f;
        case GD_Hardcore: return 1.5f;
        default: return 1.0f;
    }
}

float ABlackRemasteredGameMode::GetEnemyDamageMultiplier() const
{
    switch (CurrentDifficulty)
    {
        case GD_Recruit: return 0.75f;
        case GD_Veteran: return 1.0f;
        case GD_BlackOps: return 1.25f;
        case GD_Hardcore: return 1.5f;
        default: return 1.0f;
    }
}

float ABlackRemasteredGameMode::GetEnemyHealthMultiplier() const
{
    switch (CurrentDifficulty)
    {
        case GD_Recruit: return 0.75f;
        case GD_Veteran: return 1.0f;
        case GD_BlackOps: return 1.5f;
        case GD_Hardcore: return 2.0f;
        default: return 1.0f;
    }
}

float ABlackRemasteredGameMode::GetEnemyAccuracyMultiplier() const
{
    switch (CurrentDifficulty)
    {
        case GD_Recruit: return 0.75f;
        case GD_Veteran: return 1.0f;
        case GD_BlackOps: return 1.25f;
        case GD_Hardcore: return 1.5f;
        default: return 1.0f;
    }
}

float ABlackRemasteredGameMode::GetAmmoScarcityMultiplier() const
{
    switch (CurrentDifficulty)
    {
        case GD_Recruit: return 1.5f;
        case GD_Veteran: return 1.0f;
        case GD_BlackOps: return 0.75f;
        case GD_Hardcore: return 0.5f;
        default: return 1.0f;
    }
}

float ABlackRemasteredGameMode::GetEnemySpawnRateMultiplier() const
{
    switch (CurrentDifficulty)
    {
        case GD_Recruit: return 0.75f;
        case GD_Veteran: return 1.0f;
        case GD_BlackOps: return 1.25f;
        case GD_Hardcore: return 1.5f;
        default: return 1.0f;
    }
}

void ABlackRemasteredGameMode::StartMission(int32 MissionIndex)
{
    if (MissionIndex >= 0 && MissionIndex < MissionNames.Num())
    {
        CurrentMissionIndex = MissionIndex;
        CurrentMissionName = MissionNames[MissionIndex];
        CurrentLevelName = CurrentMissionName;
        
        // Load mission level
        LoadLevel(CurrentMissionName);
        
        // Broadcast mission started
        OnMissionStarted.Broadcast();
    }
}

void ABlackRemasteredGameMode::CompleteMission()
{
    // Broadcast mission completed
    OnMissionCompleted.Broadcast();
    
    // Save progress
    if (SaveSystem)
    {
        SaveSystem->SaveProgress(CurrentMissionIndex + 1);
    }
    
    // Load next level
    LoadNextLevel();
}

void ABlackRemasteredGameMode::CompleteGame()
{
    CurrentGameState = GS_Victory;
    
    // Broadcast game completed
    OnGameCompleted.Broadcast();
    
    // Save game
    if (SaveSystem)
    {
        SaveSystem->SaveGame();
    }
}

bool ABlackRemasteredGameMode::IsGameCompleted() const
{
    if (SaveSystem)
    {
        return SaveSystem->IsGameCompleted();
    }
    return false;
}
