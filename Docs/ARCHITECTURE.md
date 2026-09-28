# BLACK: REMASTERED - TECHNICAL ARCHITECTURE DOCUMENT

## Complete System Architecture & Code Structure

---

## TABLE OF CONTENTS
1. [OVERVIEW](#1-overview)
2. [ENGINE ARCHITECTURE](#2-engine-architecture)
3. [CORE SYSTEMS](#3-core-systems)
4. [GAMEPLAY SYSTEMS](#4-gameplay-systems)
5. [DESTRUCTION SYSTEM](#5-destruction-system)
6. [AI SYSTEM](#6-ai-system)
7. [WEAPON SYSTEM](#7-weapon-system)
8. [LEVEL SYSTEM](#8-level-system)
9. [UI SYSTEM](#9-ui-system)
10. [AUDIO SYSTEM](#10-audio-system)
11. [SAVE SYSTEM](#11-save-system)
12. [PROGRESSION SYSTEM](#12-progression-system)
13. [INPUT SYSTEM](#13-input-system)
14. [RENDERING PIPELINE](#14-rendering-pipeline)
15. [PERFORMANCE OPTIMIZATION](#15-performance-optimization)
16. [BUILD SYSTEM](#16-build-system)

---

## 1. OVERVIEW

### 1.1 Architecture Philosophy
BLACK: REMASTERED uses a **modular, data-driven architecture** with the following principles:

1. **Separation of Concerns:** Each system is isolated and communicates via well-defined interfaces
2. **Data-Driven Design:** Gameplay parameters are defined in data assets, not hardcoded
3. **Component-Based:** Actors are composed of reusable components
4. **Event-Driven:** Systems communicate via events and delegates
5. **Performance-First:** All systems are designed with performance in mind
6. **Extensibility:** Easy to add new content without modifying core systems

### 1.2 Technology Stack
- **Engine:** Unreal Engine 5.4+
- **Language:** C++ (core systems), Blueprints (gameplay logic)
- **Build System:** Unreal Build Tool (UBT) + CMake
- **Version Control:** Git
- **IDE:** Visual Studio 2022, Rider

### 1.3 Project Structure
```
BlackRemastered/
├── Config/                          # Engine and game configuration
│   ├── DefaultEngine.ini            # Engine settings
│   ├── DefaultGame.ini             # Game settings
│   ├── DefaultInput.ini            # Input bindings
│   ├── DefaultScalability.ini      # Scalability settings
│   └── ...
├── Content/                         # Game assets (not in repo)
│   ├── Characters/                  # Character assets
│   ├── Weapons/                     # Weapon assets
│   ├── Environments/                # Environment assets
│   ├── Effects/                     # VFX assets
│   ├── Sounds/                      # Audio assets
│   ├── UI/                          # UI assets
│   ├── Materials/                   # Material assets
│   ├── Blueprints/                  # Blueprint classes
│   ├── Data/                        # Data assets (JSON, etc.)
│   └── Levels/                      # Level files
├── Source/                          # C++ source code
│   └── BlackRemastered/              # Main game module
│       ├── BlackRemastered.Build.cs # Build configuration
│       ├── Private/                 # Private implementation files
│       │   ├── Core/                # Core systems
│       │   ├── Gameplay/            # Gameplay systems
│       │   ├── AI/                  # AI systems
│       │   ├── Weapons/             # Weapon systems
│       │   ├── Destruction/         # Destruction systems
│       │   ├── UI/                  # UI systems
│       │   ├── Audio/               # Audio systems
│       │   ├── Save/                # Save systems
│       │   ├── Progression/         # Progression systems
│       │   └── ...
│       ├── Public/                  # Public header files
│       │   ├── Core/                # Core system headers
│       │   ├── Gameplay/            # Gameplay system headers
│       │   ├── AI/                  # AI system headers
│       │   ├── Weapons/             # Weapon system headers
│       │   ├── Destruction/         # Destruction system headers
│       │   ├── UI/                  # UI system headers
│       │   ├── Audio/               # Audio system headers
│       │   ├── Save/                # Save system headers
│       │   ├── Progression/         # Progression system headers
│       │   └── ...
│       └── Classes/                  # Class definitions (if needed)
└── Plugins/                          # Third-party plugins
```

### 1.4 Module Dependencies
```
BlackRemastered Module Dependencies:
┌─────────────────────────────────────────────────────────┐
│                    BlackRemastered                          │
├─────────────────┬─────────────────┬──────────────────────┤
│   Core           │   Gameplay       │   Systems             │
├─────────────────┼─────────────────┼──────────────────────┤
│ - GameMode      │ - Player         │ - AIController        │
│ - GameState     │ - Character      │ - DestructionSystem   │
│ - GameInstance  │ - Weapon         │ - WeaponSystem        │
│ - SaveSystem    │ - Ammo           │ - ProgressionSystem   │
│ - InputSystem   │ - Damage         │ - AudioSystem         │
│ - ConfigSystem  │ - Health         │ - UISystem            │
└─────────────────┴─────────────────┴──────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────┐
│                    Unreal Engine Core                       │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌───────────────────┐ │
│  │  Core   │ │  Engine │ │  Render │ │    Physics         │ │
│  └─────────┘ └─────────┘ └─────────┘ └───────────────────┘ │
└─────────────────────────────────────────────────────────┘
```

---

## 2. ENGINE ARCHITECTURE

### 2.1 Engine Configuration

#### 2.1.1 DefaultEngine.ini
```ini
[/Script/Engine.Engine]
+ActiveGameName=BlackRemastered
+NetMode=Standalone

[/Script/Engine.RendererSettings]
r.DefaultFeature.AutoExposure=true
r.DefaultFeature.Bloom=true
r.DefaultFeature.MotionBlur=true
r.DefaultFeature.AmbientOcclusion=true
r.DefaultFeature.AntiAliasing=TAA
r.DefaultFeature.ScreenSpaceReflections=true
r.DefaultFeature.RayTracing=true

[/Script/Engine.RayTracingSettings]
r.RayTracing=true
r.RayTracing.Shadows=true
r.RayTracing.Reflections=true
r.RayTracing.GlobalIllumination=true
r.RayTracing.AmbientOcclusion=true

[/Script/Engine.NaniteSettings]
r.Nanite=true

[/Script/Engine.LumenSettings]
r.Lumen=true
r.Lumen.Reflections=true
r.Lumen.DiffuseIndirect=true
```

#### 2.1.2 DefaultGame.ini
```ini
[/Script/BlackRemastered.BlackRemasteredGameMode]
DefaultPlayerClass=BlackRemasteredCharacter
MaxPlayers=1
bStartPlayersAsSpectators=false

[/Script/BlackRemastered.BlackRemasteredGameState]

[/Script/BlackRemastered.BlackRemasteredPlayerState]

[/Script/Engine.GameSession]
MaxSpectators=0
MaxPlayers=1
```

### 2.2 Build Configuration

#### 2.2.1 BlackRemastered.Build.cs
```csharp
using UnrealBuildTool;

public class BlackRemastered : ModuleRules
{
    public BlackRemastered(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "NavigationSystem",
            "AIModule",
            "GameplayTasks",
            "PhysicsCore",
            "Chaos",
            "Niagara",
            "MetaSoundEngine"
        });
        
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "RenderCore",
            "RHI",
            "RuntimeFloat16_Optimized"
        });
        
        // Chaos Physics for destruction
        PrivateDependencyModuleNames.Add("Chaos");
        PrivateDependencyModuleNames.Add("ChaosCore");
        PrivateDependencyModuleNames.Add("ChaosSolvers");
        
        // Nanite for high-poly assets
        PrivateDependencyModuleNames.Add("Nanite");
        
        // Lumen for dynamic lighting
        PrivateDependencyModuleNames.Add("Lumen");
        
        // Niagara for particles
        PrivateDependencyModuleNames.Add("Niagara");
        PrivateDependencyModuleNames.Add("NiagaraCore");
        PrivateDependencyModuleNames.Add("NiagaraShader");
        
        // MetaSounds for audio
        PrivateDependencyModuleNames.Add("MetaSoundEngine");
        
        // Optimization
        bUseUnity = false;
        bUsePCHFiles = true;
        bUseRTTI = true;
        bUseExceptionHandling = false;
        bEnableUndeterminedIdentifiers = true;
        
        // Include paths
        PublicIncludePaths.Add(Path.Combine(EngineDirectory, "Source/Runtime/Engine/Public"));
        PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source/Runtime/Private"));
        PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source/Runtime/Engine/Private"));
        PrivateIncludePaths.Add("Private");
        
        // Definitions
        PublicDefinitions.Add("BLACKREMASTERED_VERSION=1");
        PublicDefinitions.Add("UE_BUILD_DEBUG=0");
        PublicDefinitions.Add("UE_BUILD_DEVELOPMENT=1");
        
        // Linker settings
        bEnableBufferVisualizationEventTrace = false;
        bEnableDataDrivenShaderPermutationReduction = true;
        
        // PCH includes
        PublicPCHHeaderFile = "Public/BlackRemastered.h";
        PrivatePCHHeaderFile = "Private/BlackRemasteredPrivatePCH.h";
        
        // Additional compiler flags
        if (Target.bDebugBuilds == true)
        {
            PrivateDefinitions.Add("DEBUG=1");
        }
        else
        {
            PrivateDefinitions.Add("DEBUG=0");
        }
    }
}
```

---

## 3. CORE SYSTEMS

### 3.1 Game Mode & Game State

#### 3.1.1 BlackRemasteredGameMode.h
```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlackRemasteredGameMode.generated.h"

class ABlackRemasteredPlayerController;
class ABlackRemasteredCharacter;
class ABlackRemasteredAIController;
class UBlackRemasteredSaveSystem;
class UBlackRemasteredProgressionSystem;

UENUM(BlueprintType)
enum class EGameDifficulty : uint8
{
    GD_Recruit UMETA(DisplayName = "Recruit"),
    GD_Veteran UMETA(DisplayName = "Veteran"),
    GD_BlackOps UMETA(DisplayName = "Black Ops"),
    GD_Hardcore UMETA(DisplayName = "Hardcore"),
    GD_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
    GS_Playing UMETA(DisplayName = "Playing"),
    GS_Paused UMETA(DisplayName = "Paused"),
    GS_GameOver UMETA(DisplayName = "Game Over"),
    GS_Victory UMETA(DisplayName = "Victory"),
    GS_Cutscene UMETA(DisplayName = "Cutscene"),
    GS_MAX UMETA(Hidden)
};

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
    
    // Game rules
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Rules")
    bool bPermadeathEnabled;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Rules")
    bool bIronmanMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Rules")
    int32 MaxLives;
    
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
    
    // Timers
    UPROPERTY()
    FTimerHandle GameTimerHandle;
    
    void OnGameTimer();
    
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
};
```

#### 3.1.2 BlackRemasteredGameMode.cpp
```cpp
#include "BlackRemasteredGameMode.h"
#include "BlackRemasteredCharacter.h"
#include "BlackRemasteredPlayerController.h"
#include "BlackRemasteredAIController.h"
#include "BlackRemasteredSaveSystem.h"
#include "BlackRemasteredProgressionSystem.h"
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
    
    // TODO: Implement level progression logic
    // For now, just restart current level
    RestartLevel();
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
```

#### 3.1.3 BlackRemasteredGameState.h
```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
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
    int32 TotalDestruction;
    
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|Stats")
    float TotalPlayTime;
    
    // Accuracy calculation
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Stats")
    float GetAccuracy() const;
    
    // K/D ratio calculation
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Stats")
    float GetKDRatio() const;
    
    // Add kill
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddKill();
    
    // Add death
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddDeath();
    
    // Add shot
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddShot(bool bHit);
    
    // Add destruction
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddDestruction(int32 Amount);
    
    // Add play time
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void AddPlayTime(float DeltaTime);
    
    // Reset statistics
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Stats")
    void ResetStatistics();
    
protected:
    // Timer for play time tracking
    UPROPERTY()
    FTimerHandle PlayTimeTimerHandle;
    
    void OnPlayTimeTimer();
};
```

#### 3.1.4 BlackRemasteredGameState.cpp
```cpp
#include "BlackRemasteredGameState.h"

ABlackRemasteredGameState::ABlackRemasteredGameState()
    : Super()
{
    CurrentLevelIndex = 0;
    TotalKills = 0;
    TotalDeaths = 0;
    TotalShotsFired = 0;
    TotalShotsHit = 0;
    TotalDestruction = 0;
    TotalPlayTime = 0.0f;
}

void ABlackRemasteredGameState::BeginPlay()
{
    Super::BeginPlay();
    
    // Start play time timer
    GetWorld()->GetTimerManager().SetTimer(
        PlayTimeTimerHandle,
        this,
        &ABlackRemasteredGameState::OnPlayTimeTimer,
        1.0f,
        true
    );
}

void ABlackRemasteredGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

float ABlackRemasteredGameState::GetAccuracy() const
{
    if (TotalShotsFired <= 0)
    {
        return 0.0f;
    }
    return static_cast<float>(TotalShotsHit) / static_cast<float>(TotalShotsFired) * 100.0f;
}

float ABlackRemasteredGameState::GetKDRatio() const
{
    if (TotalDeaths <= 0)
    {
        return static_cast<float>(TotalKills);
    }
    return static_cast<float>(TotalKills) / static_cast<float>(TotalDeaths);
}

void ABlackRemasteredGameState::AddKill()
{
    TotalKills++;
}

void ABlackRemasteredGameState::AddDeath()
{
    TotalDeaths++;
}

void ABlackRemasteredGameState::AddShot(bool bHit)
{
    TotalShotsFired++;
    if (bHit)
    {
        TotalShotsHit++;
    }
}

void ABlackRemasteredGameState::AddDestruction(int32 Amount)
{
    TotalDestruction += Amount;
}

void ABlackRemasteredGameState::AddPlayTime(float DeltaTime)
{
    TotalPlayTime += DeltaTime;
}

void ABlackRemasteredGameState::ResetStatistics()
{
    TotalKills = 0;
    TotalDeaths = 0;
    TotalShotsFired = 0;
    TotalShotsHit = 0;
    TotalDestruction = 0;
    TotalPlayTime = 0.0f;
}

void ABlackRemasteredGameState::OnPlayTimeTimer()
{
    AddPlayTime(1.0f);
}
```

---

## 4. GAMEPLAY SYSTEMS

### 4.1 Player Character

#### 4.1.1 BlackRemasteredCharacter.h
```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "BlackRemasteredCharacter.generated.h"

class UBlackRemasteredWeaponComponent;
class UBlackRemasteredHealthComponent;
class UBlackRemasteredMovementComponent;
class UBlackRemasteredInventoryComponent;
class UBlackRemasteredInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    CS_Idle UMETA(DisplayName = "Idle"),
    CS_Walking UMETA(DisplayName = "Walking"),
    CS_Running UMETA(DisplayName = "Running"),
    CS_Crouching UMETA(DisplayName = "Crouching"),
    CS_Prone UMETA(DisplayName = "Prone"),
    CS_Sliding UMETA(DisplayName = "Sliding"),
    CS_Mantling UMETA(DisplayName = "Mantling"),
    CS_InCover UMETA(DisplayName = "In Cover"),
    CS_Leaning UMETA(DisplayName = "Leaning"),
    CS_Interacting UMETA(DisplayName = "Interacting"),
    CS_Dead UMETA(DisplayName = "Dead"),
    CS_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ELeanDirection : uint8
{
    LD_None UMETA(DisplayName = "None"),
    LD_Left UMETA(DisplayName = "Left"),
    LD_Right UMETA(DisplayName = "Right"),
    LD_MAX UMETA(Hidden)
};

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABlackRemasteredCharacter();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;
    
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredWeaponComponent* WeaponComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredHealthComponent* HealthComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredMovementComponent* MovementComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredInventoryComponent* InventoryComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Components")
    UBlackRemasteredInteractionComponent* InteractionComponent;
    
    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Camera")
    USpringArmComponent* CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Camera")
    UCameraComponent* FollowCamera;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    ECharacterState CharacterState;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    ELeanDirection LeanDirection;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsAiming;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsSprinting;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsCrouching;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|State")
    bool bIsProne;
    
    // Movement settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float WalkSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float RunSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float CrouchSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float ProneSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float SprintSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float SlideSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Movement")
    float MantleSpeed;
    
    // Camera settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraFOV;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float AimFOV;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraLagSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraLagMaxDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    FVector CameraOffset;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    FVector AimCameraOffset;
    
    // Input bindings
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void MoveForward(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void MoveRight(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void LookUp(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void LookRight(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartSprint();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopSprint();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartCrouch();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopCrouch();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartProne();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopProne();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartSlide();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartAim();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopAim();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartLeanLeft();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopLeanLeft();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StartLeanRight();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void StopLeanRight();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Jump();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Mantle();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Interact();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Reload();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Fire();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Aim();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void Melee();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void ThrowGrenade();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void NextWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void PreviousWeapon();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void SelectWeapon(int32 Index);
    
    // State management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|State")
    void SetCharacterState(ECharacterState NewState);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|State")
    void SetLeanDirection(ELeanDirection NewDirection);
    
    // Camera management
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Camera")
    void UpdateCamera(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Camera")
    void ApplyCameraEffects(float DeltaTime);
    
    // Movement helpers
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanSprint() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanCrouch() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanProne() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanSlide() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanMantle() const;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Movement")
    bool CanLean() const;
    
    // Damage
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Damage")
    void TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
    
    // Death
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Death")
    void Die(AController* Killer);
    
    // Respawn
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Respawn")
    void Respawn();
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredHealthComponent* GetHealthComponent() const { return HealthComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredMovementComponent* GetMovementComponent() const { return MovementComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UBlackRemasteredInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsAiming() const { return bIsAiming; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsSprinting() const { return bIsSprinting; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsCrouching() const { return bIsCrouching; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    bool IsProne() const { return bIsProne; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    ECharacterState GetCharacterState() const { return CharacterState; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    ELeanDirection GetLeanDirection() const { return LeanDirection; }
    
protected:
    // Input component
    UPROPERTY()
    UInputComponent* InputComponent;
    
    // Movement input
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    float MoveForwardValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    float MoveRightValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    float LookUpValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    float LookRightValue;
    
    // Action input
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bSprintInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bCrouchInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bProneInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bAimInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bLeanLeftInput;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Input")
    bool bLeanRightInput;
    
    // Timers
    UPROPERTY()
    FTimerHandle SprintTimerHandle;
    
    UPROPERTY()
    FTimerHandle SlideTimerHandle;
    
    UPROPERTY()
    FTimerHandle MantleTimerHandle;
    
    // Stamina
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float MaxStamina;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Stamina")
    float CurrentStamina;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float StaminaRegenRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float StaminaRegenDelay;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float SprintStaminaCost;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Stamina")
    float SlideStaminaCost;
    
    // Camera effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraSwayAmount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraSwaySpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraBobAmount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|Camera")
    float CameraBobSpeed;
    
    // Helper functions
    void UpdateMovement(float DeltaTime);
    void UpdateStamina(float DeltaTime);
    void UpdateState(float DeltaTime);
    
    void OnSprintTimer();
    void OnSlideTimer();
    void OnMantleTimer();
    
    void ApplyMovementPenalties();
    void ApplyCameraSway(float DeltaTime);
    void ApplyCameraBob(float DeltaTime);
    
    // Cover system
    void CheckForCover();
    void EnterCover();
    void ExitCover();
    
    // Lean system
    void UpdateLean(float DeltaTime);
    
    // Animation
    void UpdateAnimation();
};
```

This is the beginning of the architecture document. Due to length constraints, I'll continue with the remaining systems in the next file.
