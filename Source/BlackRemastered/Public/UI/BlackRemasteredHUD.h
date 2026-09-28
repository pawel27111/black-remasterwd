#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlackRemastered.h"
#include "BlackRemasteredHUD.generated.h"

class ABlackRemasteredCharacter;
class ABlackRemasteredPlayerController;
class UBlackRemasteredGameMode;
class UUserWidget;
class UTexture2D;

UENUM(BlueprintType)
enum class EHUDState : uint8
{
    HUD_None UMETA(DisplayName = "None"),
    HUD_Playing UMETA(DisplayName = "Playing"),
    HUD_Paused UMETA(DisplayName = "Paused"),
    HUD_Menu UMETA(DisplayName = "Menu"),
    HUD_Inventory UMETA(DisplayName = "Inventory"),
    HUD_Map UMETA(DisplayName = "Map"),
    HUD_Cutscene UMETA(DisplayName = "Cutscene"),
    HUD_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EHUDElement : uint8
{
    HUD_HealthBar UMETA(DisplayName = "Health Bar"),
    HUD_ArmorBar UMETA(DisplayName = "Armor Bar"),
    HUD_AmmoCounter UMETA(DisplayName = "Ammo Counter"),
    HUD_WeaponIcon UMETA(DisplayName = "Weapon Icon"),
    HUD_Crosshair UMETA(DisplayName = "Crosshair"),
    HUD_MiniMap UMETA(DisplayName = "Mini-Map"),
    HUD_ObjectiveMarker UMETA(DisplayName = "Objective Marker"),
    HUD_DamageIndicator UMETA(DisplayName = "Damage Indicator"),
    HUD_StaminaBar UMETA(DisplayName = "Stamina Bar"),
    HUD_Compass UMETA(DisplayName = "Compass"),
    HUD_MessageLog UMETA(DisplayName = "Message Log"),
    HUD_WeaponWheel UMETA(DisplayName = "Weapon Wheel"),
    HUD_InteractionPrompt UMETA(DisplayName = "Interaction Prompt"),
    HUD_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FHUDColor
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Color")
    FLinearColor Primary;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Color")
    FLinearColor Secondary;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Color")
    FLinearColor Background;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Color")
    FLinearColor Text;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Color")
    FLinearColor Warning;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Color")
    FLinearColor Success;
};

USTRUCT(BlueprintType)
struct FHUDSettings
{
    GENERATED_BODY()
    
    // Visibility
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowHealthBar;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowArmorBar;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowAmmoCounter;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowWeaponIcon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowCrosshair;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowMiniMap;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowObjectiveMarker;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowDamageIndicator;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowStaminaBar;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowCompass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Visibility")
    bool bShowMessageLog;
    
    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
    FHUDColor Colors;
    
    // Sizes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Sizes")
    float HealthBarWidth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Sizes")
    float HealthBarHeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Sizes")
    float ArmorBarWidth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Sizes")
    float ArmorBarHeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Sizes")
    float AmmoCounterFontSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Sizes")
    float WeaponIconSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Sizes")
    float CrosshairSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Sizes")
    float MiniMapSize;
    
    // Positions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Positions")
    FVector2D HealthBarPosition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Positions")
    FVector2D ArmorBarPosition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Positions")
    FVector2D AmmoCounterPosition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Positions")
    FVector2D WeaponIconPosition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Positions")
    FVector2D MiniMapPosition;
    
    // Opacity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Opacity")
    float HUDOpacity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Opacity")
    float LowHealthOpacity;
    
    // Animations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Animations")
    float HealthBarPulseSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Animations")
    float DamageIndicatorDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Animations")
    float MessageLogDuration;
};

UCLASS()
class BLACKREMASTERED_API ABlackRemasteredHUD : public AHUD
{
    GENERATED_BODY()

public:
    ABlackRemasteredHUD();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void DrawHUD() override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void Initialize(ABlackRemasteredPlayerController* PlayerController);
    
    // HUD state
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void SetHUDState(EHUDState NewState);
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|HUD")
    EHUDState GetHUDState() const { return CurrentHUDState; }
    
    // HUD elements
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ShowHUD();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void HideHUD();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ToggleHUD();
    
    // Health
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void UpdateHealth(float CurrentHealth, float MaxHealth);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void UpdateArmor(float CurrentArmor, float MaxArmor);
    
    // Ammo
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void UpdateAmmo(int32 CurrentAmmo, int32 ReserveAmmo, int32 MagazineCapacity);
    
    // Weapon
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void UpdateWeapon(EWeaponType WeaponType, int32 WeaponIndex);
    
    // Crosshair
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void UpdateCrosshair(EWeaponType WeaponType, bool bIsAiming);
    
    // Stamina
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void UpdateStamina(float CurrentStamina, float MaxStamina);
    
    // Damage
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ShowDamageIndicator(float DamageAmount, FVector DamageDirection);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void HideDamageIndicator();
    
    // Messages
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void AddMessage(const FText& Message, FLinearColor Color = FLinearColor::White);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ClearMessages();
    
    // Objectives
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void UpdateObjective(const FText& ObjectiveText);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ClearObjective();
    
    // Mini-map
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void UpdateMiniMap();
    
    // Interaction
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ShowInteractionPrompt(const FText& Prompt);
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void HideInteractionPrompt();
    
    // UI Widgets
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ShowMainMenu();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void HideMainMenu();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ShowPauseMenu();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void HidePauseMenu();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ShowInventory();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void HideInventory();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ShowMap();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void HideMap();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void ShowOptionsMenu();
    
    UFUNCTION(BlueprintCallable, Category = "BlackRemastered|HUD")
    void HideOptionsMenu();
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|HUD")
    ABlackRemasteredPlayerController* GetPlayerController() const { return PlayerController; }
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|HUD")
    ABlackRemasteredCharacter* GetPlayerCharacter() const;
    
    UFUNCTION(BlueprintPure, Category = "BlackRemastered|HUD")
    FHUDSettings GetHUDSettings() const { return HUDSettings; }
    
    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|HUD")
    FHUDSettings HUDSettings;
    
    // UI Widgets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|HUD")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|HUD")
    TSubclassOf<UUserWidget> PauseMenuWidgetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|HUD")
    TSubclassOf<UUserWidget> InventoryWidgetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|HUD")
    TSubclassOf<UUserWidget> MapWidgetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackRemastered|HUD")
    TSubclassOf<UUserWidget> OptionsWidgetClass;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHUDStateChanged);
    UPROPERTY(BlueprintAssignable, Category = "BlackRemastered|HUD")
    FOnHUDStateChanged OnHUDStateChanged;
    
protected:
    // Player controller
    UPROPERTY()
    ABlackRemasteredPlayerController* PlayerController;
    
    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    EHUDState CurrentHUDState;
    
    // UI Widgets
    UPROPERTY()
    UUserWidget* MainMenuWidget;
    
    UPROPERTY()
    UUserWidget* PauseMenuWidget;
    
    UPROPERTY()
    UUserWidget* InventoryWidget;
    
    UPROPERTY()
    UUserWidget* MapWidget;
    
    UPROPERTY()
    UUserWidget* OptionsWidget;
    
    // HUD Data
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    float CurrentHealth;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    float MaxHealth;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    float CurrentArmor;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    float MaxArmor;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    int32 CurrentAmmo;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    int32 ReserveAmmo;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    int32 MagazineCapacity;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    EWeaponType CurrentWeaponType;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    float CurrentStamina;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    float MaxStamina;
    
    // Damage indicator
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    bool bShowDamageIndicator;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    FVector2D DamageIndicatorDirection;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    float DamageIndicatorIntensity;
    
    // Messages
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    TArray<FText> MessageLog;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    TArray<FLinearColor> MessageColors;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    TArray<float> MessageTimers;
    
    // Objective
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    FText CurrentObjective;
    
    // Interaction
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    FText InteractionPrompt;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackRemastered|HUD")
    bool bShowInteractionPrompt;
    
    // Timers
    UPROPERTY()
    FTimerHandle DamageIndicatorTimerHandle;
    
    UPROPERTY()
    FTimerHandle MessageTimerHandle;
    
    // Helper functions
    void OnDamageIndicatorTimer();
    void OnMessageTimer();
    
    void DrawHealthBar();
    void DrawArmorBar();
    void DrawAmmoCounter();
    void DrawWeaponIcon();
    void DrawCrosshair();
    void DrawMiniMap();
    void DrawObjectiveMarker();
    void DrawDamageIndicator();
    void DrawStaminaBar();
    void DrawCompass();
    void DrawMessageLog();
    void DrawInteractionPrompt();
    
    FVector2D GetScreenPosition(FVector WorldPosition) const;
    
    void UpdateHUDData();
};
