#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlackRemastered.h"
#include "BlackRemasteredPlayerController.generated.h"

class ABlackRemasteredCharacter;
class ABlackRemasteredGameMode;
class UBlackRemasteredHUD;

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ABlackRemasteredPlayerController();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Player")
    void InitializePlayer();
    
    // HUD
    UPROPERTY(BlueprintReadOnly, Category = "BlackRemastered|UI")
    UBlackRemasteredHUD* BlackRemasteredHUD;
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|UI")
    UBlackRemasteredHUD* GetBlackRemasteredHUD() const { return BlackRemasteredHUD; }
    
    // Input
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void TogglePause();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void OpenMenu();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void OpenInventory();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void OpenMap();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void QuickSave();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Input")
    void QuickLoad();
    
    // Game state
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Game")
    void SetGamePaused(bool bPaused);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Game")
    bool IsGamePaused() const;
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    ABlackRemasteredCharacter* GetBlackRemasteredCharacter() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Getters")
    ABlackRemasteredGameMode* GetBlackRemasteredGameMode() const;
    
    // Difficulty
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|Difficulty")
    void SetDifficulty(EGameDifficulty Difficulty);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|Difficulty")
    EGameDifficulty GetDifficulty() const;
    
protected:
    // Input bindings
    virtual void BindInputActions();
    
    // Pause state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|Game")
    bool bIsPaused;
    
    // UI state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|UI")
    bool bIsMenuOpen;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|UI")
    bool bIsInventoryOpen;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|UI")
    bool bIsMapOpen;
    
    // Input component
    UPROPERTY()
    UInputComponent* InputComponent;
};
