#include "BlackRemasteredPlayerController.h"
#include "BlackRemasteredCharacter.h"
#include "Core/BlackRemasteredGameMode.h"
#include "UI/BlackRemasteredHUD.h"
#include "Save/BlackRemasteredSaveSystem.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

ABlackRemasteredPlayerController::ABlackRemasteredPlayerController()
    : Super()
{
    bIsPaused = false;
    bIsMenuOpen = false;
    bIsInventoryOpen = false;
    bIsMapOpen = false;
}

void ABlackRemasteredPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize player
    InitializePlayer();
}

void ABlackRemasteredPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABlackRemasteredPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    InputComponent = GetInputComponent();
    
    if (InputComponent)
    {
        BindInputActions();
    }
}

void ABlackRemasteredPlayerController::InitializePlayer()
{
    // Create HUD
    if (!BlackRemasteredHUD)
    {
        BlackRemasteredHUD = NewObject<UBlackRemasteredHUD>(this);
        BlackRemasteredHUD->Initialize(this);
    }
    
    // Set initial game state
    if (ABlackRemasteredGameMode* GameMode = GetBlackRemasteredGameMode())
    {
        GameMode->SetGameState(GS_Playing);
    }
}

void ABlackRemasteredPlayerController::BindInputActions()
{
    if (!InputComponent)
    {
        return;
    }
    
    // Pause
    InputComponent->BindAction("Pause", IE_Pressed, this, &ABlackRemasteredPlayerController::TogglePause);
    
    // Menu
    InputComponent->BindAction("Menu", IE_Pressed, this, &ABlackRemasteredPlayerController::OpenMenu);
    
    // Inventory
    InputComponent->BindAction("Inventory", IE_Pressed, this, &ABlackRemasteredPlayerController::OpenInventory);
    
    // Map
    InputComponent->BindAction("Map", IE_Pressed, this, &ABlackRemasteredPlayerController::OpenMap);
    
    // Quick save/load
    InputComponent->BindAction("QuickSave", IE_Pressed, this, &ABlackRemasteredPlayerController::QuickSave);
    InputComponent->BindAction("QuickLoad", IE_Pressed, this, &ABlackRemasteredPlayerController::QuickLoad);
}

void ABlackRemasteredPlayerController::TogglePause()
{
    bIsPaused = !bIsPaused;
    SetGamePaused(bIsPaused);
    
    if (bIsPaused)
    {
        // Open pause menu
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->ShowPauseMenu();
        }
    }
    else
    {
        // Close pause menu
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->HidePauseMenu();
        }
    }
}

void ABlackRemasteredPlayerController::OpenMenu()
{
    bIsMenuOpen = !bIsMenuOpen;
    
    if (bIsMenuOpen)
    {
        // Open main menu
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->ShowMainMenu();
        }
    }
    else
    {
        // Close main menu
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->HideMainMenu();
        }
    }
}

void ABlackRemasteredPlayerController::OpenInventory()
{
    bIsInventoryOpen = !bIsInventoryOpen;
    
    if (bIsInventoryOpen)
    {
        // Open inventory
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->ShowInventory();
        }
        
        // Pause game
        SetGamePaused(true);
    }
    else
    {
        // Close inventory
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->HideInventory();
        }
        
        // Resume game
        SetGamePaused(false);
    }
}

void ABlackRemasteredPlayerController::OpenMap()
{
    bIsMapOpen = !bIsMapOpen;
    
    if (bIsMapOpen)
    {
        // Open map
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->ShowMap();
        }
        
        // Pause game
        SetGamePaused(true);
    }
    else
    {
        // Close map
        if (BlackRemasteredHUD)
        {
            BlackRemasteredHUD->HideMap();
        }
        
        // Resume game
        SetGamePaused(false);
    }
}

void ABlackRemasteredPlayerController::QuickSave()
{
    if (ABlackRemasteredGameMode* GameMode = GetBlackRemasteredGameMode())
    {
        if (GameMode->SaveSystem)
        {
            GameMode->SaveSystem->QuickSave();
        }
    }
}

void ABlackRemasteredPlayerController::QuickLoad()
{
    if (ABlackRemasteredGameMode* GameMode = GetBlackRemasteredGameMode())
    {
        if (GameMode->SaveSystem)
        {
            GameMode->SaveSystem->QuickLoad();
        }
    }
}

void ABlackRemasteredPlayerController::SetGamePaused(bool bPaused)
{
    bIsPaused = bPaused;
    UGameplayStatics::SetGamePaused(GetWorld(), bPaused);
    
    if (ABlackRemasteredGameMode* GameMode = GetBlackRemasteredGameMode())
    {
        if (bPaused)
        {
            GameMode->SetGameState(GS_Paused);
        }
        else
        {
            GameMode->SetGameState(GS_Playing);
        }
    }
}

bool ABlackRemasteredPlayerController::IsGamePaused() const
{
    return bIsPaused;
}

ABlackRemasteredCharacter* ABlackRemasteredPlayerController::GetBlackRemasteredCharacter() const
{
    if (APawn* Pawn = GetPawn())
    {
        return Cast<ABlackRemasteredCharacter>(Pawn);
    }
    return nullptr;
}

ABlackRemasteredGameMode* ABlackRemasteredPlayerController::GetBlackRemasteredGameMode() const
{
    if (UWorld* World = GetWorld())
    {
        return Cast<ABlackRemasteredGameMode>(World->GetAuthGameMode());
    }
    return nullptr;
}

void ABlackRemasteredPlayerController::SetDifficulty(EGameDifficulty Difficulty)
{
    if (ABlackRemasteredGameMode* GameMode = GetBlackRemasteredGameMode())
    {
        GameMode->SetDifficulty(Difficulty);
    }
}

EGameDifficulty ABlackRemasteredPlayerController::GetDifficulty() const
{
    if (ABlackRemasteredGameMode* GameMode = GetBlackRemasteredGameMode())
    {
        return GameMode->GetDifficulty();
    }
    return GD_Veteran;
}
