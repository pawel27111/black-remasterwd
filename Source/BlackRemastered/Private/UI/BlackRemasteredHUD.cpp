#include "BlackRemasteredHUD.h"
#include "BlackRemasteredCharacter.h"
#include "BlackRemasteredPlayerController.h"
#include "Gameplay/BlackRemasteredHealthComponent.h"
#include "Gameplay/BlackRemasteredWeaponComponent.h"
#include "Gameplay/BlackRemasteredInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Kismet/GameplayStatics.h"

ABlackRemasteredHUD::ABlackRemasteredHUD()
    : Super()
{
    // Initialize state
    PlayerController = nullptr;
    CurrentHUDState = HUD_None;
    
    // Initialize HUD data
    CurrentHealth = 100.0f;
    MaxHealth = 100.0f;
    CurrentArmor = 0.0f;
    MaxArmor = 100.0f;
    CurrentAmmo = 30;
    ReserveAmmo = 90;
    MagazineCapacity = 30;
    CurrentWeaponType = WT_AssaultRifle;
    CurrentStamina = 100.0f;
    MaxStamina = 100.0f;
    
    // Initialize damage indicator
    bShowDamageIndicator = false;
    DamageIndicatorDirection = FVector2D::ZeroVector;
    DamageIndicatorIntensity = 0.0f;
    
    // Initialize messages
    CurrentObjective = FText::GetEmpty();
    bShowInteractionPrompt = false;
    
    // Initialize HUD settings
    HUDSettings.bShowHealthBar = true;
    HUDSettings.bShowArmorBar = true;
    HUDSettings.bShowAmmoCounter = true;
    HUDSettings.bShowWeaponIcon = true;
    HUDSettings.bShowCrosshair = true;
    HUDSettings.bShowMiniMap = true;
    HUDSettings.bShowObjectiveMarker = true;
    HUDSettings.bShowDamageIndicator = true;
    HUDSettings.bShowStaminaBar = true;
    HUDSettings.bShowCompass = true;
    HUDSettings.bShowMessageLog = true;
    
    // Colors
    HUDSettings.Colors.Primary = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
    HUDSettings.Colors.Secondary = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
    HUDSettings.Colors.Background = FLinearColor(0.2f, 0.2f, 0.2f, 0.8f);
    HUDSettings.Colors.Text = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
    HUDSettings.Colors.Warning = FLinearColor(1.0f, 0.3f, 0.3f, 1.0f);
    HUDSettings.Colors.Success = FLinearColor(0.3f, 1.0f, 0.3f, 1.0f);
    
    // Sizes
    HUDSettings.HealthBarWidth = 200.0f;
    HUDSettings.HealthBarHeight = 20.0f;
    HUDSettings.ArmorBarWidth = 200.0f;
    HUDSettings.ArmorBarHeight = 10.0f;
    HUDSettings.AmmoCounterFontSize = 16;
    HUDSettings.WeaponIconSize = 32.0f;
    HUDSettings.CrosshairSize = 10.0f;
    HUDSettings.MiniMapSize = 150.0f;
    
    // Positions
    HUDSettings.HealthBarPosition = FVector2D(20.0f, 20.0f);
    HUDSettings.ArmorBarPosition = FVector2D(20.0f, 45.0f);
    HUDSettings.AmmoCounterPosition = FVector2D(0.0f, 0.0f); // Center
    HUDSettings.WeaponIconPosition = FVector2D(0.0f, 0.0f); // Center
    HUDSettings.MiniMapPosition = FVector2D(Canvas->SizeX - 170.0f, 20.0f);
    
    // Opacity
    HUDSettings.HUDOpacity = 1.0f;
    HUDSettings.LowHealthOpacity = 0.8f;
    
    // Animations
    HUDSettings.HealthBarPulseSpeed = 2.0f;
    HUDSettings.DamageIndicatorDuration = 0.5f;
    HUDSettings.MessageLogDuration = 5.0f;
}

void ABlackRemasteredHUD::BeginPlay()
{
    Super::BeginPlay();
}

void ABlackRemasteredHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update HUD data
    UpdateHUDData();
}

void ABlackRemasteredHUD::DrawHUD()
{
    Super::DrawHUD();
    
    if (CurrentHUDState == HUD_None || CurrentHUDState == HUD_Paused || CurrentHUDState == HUD_Menu)
    {
        return;
    }
    
    // Draw HUD elements
    if (HUDSettings.bShowHealthBar)
    {
        DrawHealthBar();
    }
    
    if (HUDSettings.bShowArmorBar)
    {
        DrawArmorBar();
    }
    
    if (HUDSettings.bShowAmmoCounter)
    {
        DrawAmmoCounter();
    }
    
    if (HUDSettings.bShowWeaponIcon)
    {
        DrawWeaponIcon();
    }
    
    if (HUDSettings.bShowCrosshair)
    {
        DrawCrosshair();
    }
    
    if (HUDSettings.bShowStaminaBar)
    {
        DrawStaminaBar();
    }
    
    if (HUDSettings.bShowMiniMap)
    {
        DrawMiniMap();
    }
    
    if (HUDSettings.bShowCompass)
    {
        DrawCompass();
    }
    
    if (HUDSettings.bShowObjectiveMarker)
    {
        DrawObjectiveMarker();
    }
    
    if (bShowDamageIndicator && HUDSettings.bShowDamageIndicator)
    {
        DrawDamageIndicator();
    }
    
    if (HUDSettings.bShowMessageLog)
    {
        DrawMessageLog();
    }
    
    if (bShowInteractionPrompt)
    {
        DrawInteractionPrompt();
    }
}

void ABlackRemasteredHUD::Initialize(ABlackRemasteredPlayerController* Controller)
{
    PlayerController = Controller;
    
    // Set initial HUD state
    SetHUDState(HUD_Playing);
}

void ABlackRemasteredHUD::SetHUDState(EHUDState NewState)
{
    EHUDState OldState = CurrentHUDState;
    CurrentHUDState = NewState;
    
    // Handle state transitions
    switch (NewState)
    {
        case HUD_Playing:
            ShowHUD();
            break;
            
        case HUD_Paused:
            // Keep HUD visible but don't update
            break;
            
        case HUD_Menu:
            HideHUD();
            break;
            
        case HUD_Inventory:
            HideHUD();
            break;
            
        case HUD_Map:
            HideHUD();
            break;
            
        default:
            break;
    }
    
    // Broadcast state change
    OnHUDStateChanged.Broadcast();
}

void ABlackRemasteredHUD::ShowHUD()
{
    CurrentHUDState = HUD_Playing;
}

void ABlackRemasteredHUD::HideHUD()
{
    CurrentHUDState = HUD_None;
}

void ABlackRemasteredHUD::ToggleHUD()
{
    if (CurrentHUDState == HUD_None || CurrentHUDState == HUD_Menu || CurrentHUDState == HUD_Inventory || CurrentHUDState == HUD_Map)
    {
        ShowHUD();
    }
    else
    {
        HideHUD();
    }
}

void ABlackRemasteredHUD::UpdateHealth(float Current, float Max)
{
    CurrentHealth = Current;
    MaxHealth = Max;
}

void ABlackRemasteredHUD::UpdateArmor(float Current, float Max)
{
    CurrentArmor = Current;
    MaxArmor = Max;
}

void ABlackRemasteredHUD::UpdateAmmo(int32 Current, int32 Reserve, int32 MagazineCap)
{
    CurrentAmmo = Current;
    ReserveAmmo = Reserve;
    MagazineCapacity = MagazineCap;
}

void ABlackRemasteredHUD::UpdateWeapon(EWeaponType WeaponType, int32 WeaponIndex)
{
    CurrentWeaponType = WeaponType;
}

void ABlackRemasteredHUD::UpdateCrosshair(EWeaponType WeaponType, bool bIsAiming)
{
    CurrentWeaponType = WeaponType;
    
    // Update crosshair based on weapon type and aim state
    // This is handled in DrawCrosshair()
}

void ABlackRemasteredHUD::UpdateStamina(float Current, float Max)
{
    CurrentStamina = Current;
    MaxStamina = Max;
}

void ABlackRemasteredHUD::ShowDamageIndicator(float DamageAmount, FVector DamageDirection)
{
    bShowDamageIndicator = true;
    DamageIndicatorIntensity = DamageAmount / 50.0f; // Normalize
    
    // Convert world direction to screen direction
    FVector2D ScreenSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ScreenSize);
    }
    else
    {
        ScreenSize = FVector2D(1920.0f, 1080.0f);
    }
    
    // Calculate screen direction
    FVector2D ScreenCenter = ScreenSize * 0.5f;
    FVector2D ScreenPosition = GetScreenPosition(DamageDirection * 1000.0f + GetOwningPawn()->GetActorLocation());
    
    DamageIndicatorDirection = (ScreenPosition - ScreenCenter).GetSafeNormal();
    
    // Start timer
    GetWorld()->GetTimerManager().SetTimer(
        DamageIndicatorTimerHandle,
        this,
        &ABlackRemasteredHUD::OnDamageIndicatorTimer,
        HUDSettings.DamageIndicatorDuration,
        false
    );
}

void ABlackRemasteredHUD::HideDamageIndicator()
{
    bShowDamageIndicator = false;
    DamageIndicatorIntensity = 0.0f;
}

void ABlackRemasteredHUD::AddMessage(const FText& Message, FLinearColor Color)
{
    // Add message to log
    MessageLog.Add(Message);
    MessageColors.Add(Color);
    MessageTimers.Add(0.0f);
    
    // Limit message log size
    if (MessageLog.Num() > 10)
    {
        MessageLog.RemoveAt(0);
        MessageColors.RemoveAt(0);
        MessageTimers.RemoveAt(0);
    }
}

void ABlackRemasteredHUD::ClearMessages()
{
    MessageLog.Empty();
    MessageColors.Empty();
    MessageTimers.Empty();
}

void ABlackRemasteredHUD::UpdateObjective(const FText& ObjectiveText)
{
    CurrentObjective = ObjectiveText;
}

void ABlackRemasteredHUD::ClearObjective()
{
    CurrentObjective = FText::GetEmpty();
}

void ABlackRemasteredHUD::UpdateMiniMap()
{
    // Update mini-map
    // This would be handled by the mini-map widget
}

void ABlackRemasteredHUD::ShowInteractionPrompt(const FText& Prompt)
{
    InteractionPrompt = Prompt;
    bShowInteractionPrompt = true;
}

void ABlackRemasteredHUD::HideInteractionPrompt()
{
    bShowInteractionPrompt = false;
}

void ABlackRemasteredHUD::ShowMainMenu()
{
    if (!MainMenuWidgetClass)
    {
        return;
    }
    
    // Hide other menus
    HidePauseMenu();
    HideInventory();
    HideMap();
    HideOptionsMenu();
    
    // Create or show main menu
    if (!MainMenuWidget)
    {
        MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
        }
    }
    else
    {
        MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
    }
    
    SetHUDState(HUD_Menu);
}

void ABlackRemasteredHUD::HideMainMenu()
{
    if (MainMenuWidget)
    {
        MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    SetHUDState(HUD_Playing);
}

void ABlackRemasteredHUD::ShowPauseMenu()
{
    if (!PauseMenuWidgetClass)
    {
        return;
    }
    
    // Hide other menus
    HideMainMenu();
    HideInventory();
    HideMap();
    HideOptionsMenu();
    
    // Create or show pause menu
    if (!PauseMenuWidget)
    {
        PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
        if (PauseMenuWidget)
        {
            PauseMenuWidget->AddToViewport();
        }
    }
    else
    {
        PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
    }
    
    SetHUDState(HUD_Paused);
}

void ABlackRemasteredHUD::HidePauseMenu()
{
    if (PauseMenuWidget)
    {
        PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    SetHUDState(HUD_Playing);
}

void ABlackRemasteredHUD::ShowInventory()
{
    if (!InventoryWidgetClass)
    {
        return;
    }
    
    // Hide other menus
    HideMainMenu();
    HidePauseMenu();
    HideMap();
    HideOptionsMenu();
    
    // Create or show inventory
    if (!InventoryWidget)
    {
        InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport();
        }
    }
    else
    {
        InventoryWidget->SetVisibility(ESlateVisibility::Visible);
    }
    
    SetHUDState(HUD_Inventory);
}

void ABlackRemasteredHUD::HideInventory()
{
    if (InventoryWidget)
    {
        InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    SetHUDState(HUD_Playing);
}

void ABlackRemasteredHUD::ShowMap()
{
    if (!MapWidgetClass)
    {
        return;
    }
    
    // Hide other menus
    HideMainMenu();
    HidePauseMenu();
    HideInventory();
    HideOptionsMenu();
    
    // Create or show map
    if (!MapWidget)
    {
        MapWidget = CreateWidget<UUserWidget>(GetWorld(), MapWidgetClass);
        if (MapWidget)
        {
            MapWidget->AddToViewport();
        }
    }
    else
    {
        MapWidget->SetVisibility(ESlateVisibility::Visible);
    }
    
    SetHUDState(HUD_Map);
}

void ABlackRemasteredHUD::HideMap()
{
    if (MapWidget)
    {
        MapWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    SetHUDState(HUD_Playing);
}

void ABlackRemasteredHUD::ShowOptionsMenu()
{
    if (!OptionsWidgetClass)
    {
        return;
    }
    
    // Hide other menus
    HideMainMenu();
    HidePauseMenu();
    HideInventory();
    HideMap();
    
    // Create or show options menu
    if (!OptionsWidget)
    {
        OptionsWidget = CreateWidget<UUserWidget>(GetWorld(), OptionsWidgetClass);
        if (OptionsWidget)
        {
            OptionsWidget->AddToViewport();
        }
    }
    else
    {
        OptionsWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void ABlackRemasteredHUD::HideOptionsMenu()
{
    if (OptionsWidget)
    {
        OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    SetHUDState(HUD_Playing);
}

ABlackRemasteredCharacter* ABlackRemasteredHUD::GetPlayerCharacter() const
{
    if (PlayerController)
    {
        return PlayerController->GetBlackRemasteredCharacter();
    }
    return nullptr;
}

// Timer callbacks
void ABlackRemasteredHUD::OnDamageIndicatorTimer()
{
    HideDamageIndicator();
}

void ABlackRemasteredHUD::OnMessageTimer()
{
    // Update message timers
    for (int32 i = MessageTimers.Num() - 1; i >= 0; i--)
    {
        MessageTimers[i] += GetWorld()->GetDeltaSeconds();
        
        if (MessageTimers[i] >= HUDSettings.MessageLogDuration)
        {
            MessageTimers.RemoveAt(i);
            MessageLog.RemoveAt(i);
            MessageColors.RemoveAt(i);
        }
    }
}

// Draw functions
void ABlackRemasteredHUD::DrawHealthBar()
{
    if (!Canvas)
    {
        return;
    }
    
    // Calculate health percentage
    float HealthPercentage = CurrentHealth / MaxHealth;
    
    // Calculate bar dimensions
    float BarWidth = HUDSettings.HealthBarWidth;
    float BarHeight = HUDSettings.HealthBarHeight;
    float BarX = HUDSettings.HealthBarPosition.X;
    float BarY = HUDSettings.HealthBarPosition.Y;
    
    // Calculate fill width
    float FillWidth = BarWidth * HealthPercentage;
    
    // Draw background
    FCanvasBoxItem BackgroundBox(FVector2D(BarX, BarY), FVector2D(BarWidth, BarHeight));
    BackgroundBox.SetColor(HUDSettings.Colors.Background);
    Canvas->DrawItem(BackgroundBox);
    
    // Draw fill
    FCanvasBoxItem FillBox(FVector2D(BarX, BarY), FVector2D(FillWidth, BarHeight));
    
    // Change color based on health percentage
    FLinearColor FillColor;
    if (HealthPercentage > 0.6f)
    {
        FillColor = HUDSettings.Colors.Success;
    }
    else if (HealthPercentage > 0.3f)
    {
        FillColor = HUDSettings.Colors.Secondary;
    }
    else
    {
        FillColor = HUDSettings.Colors.Warning;
        
        // Pulse effect for low health
        float Pulse = FMath::Sin(GetWorld()->GetTimeSeconds() * HUDSettings.HealthBarPulseSpeed) * 0.2f + 0.8f;
        FillColor.A = Pulse;
    }
    
    FillBox.SetColor(FillColor);
    Canvas->DrawItem(FillBox);
    
    // Draw border
    FCanvasBoxItem BorderBox(FVector2D(BarX, BarY), FVector2D(BarWidth, BarHeight));
    BorderBox.SetColor(HUDSettings.Colors.Primary);
    BorderBox.LineThickness = 1.0f;
    Canvas->DrawItem(BorderBox);
    
    // Draw health text
    FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::RoundToInt(CurrentHealth), FMath::RoundToInt(MaxHealth));
    FCanvasTextItem HealthTextItem(FVector2D(BarX + BarWidth + 10.0f, BarY), FText::FromString(HealthText), GEngine->GetSmallFont(), FLinearColor::White);
    HealthTextItem.Scale = FVector2D(0.8f, 0.8f);
    Canvas->DrawItem(HealthTextItem);
}

void ABlackRemasteredHUD::DrawArmorBar()
{
    if (!Canvas || MaxArmor <= 0)
    {
        return;
    }
    
    // Calculate armor percentage
    float ArmorPercentage = CurrentArmor / MaxArmor;
    
    // Calculate bar dimensions
    float BarWidth = HUDSettings.ArmorBarWidth;
    float BarHeight = HUDSettings.ArmorBarHeight;
    float BarX = HUDSettings.ArmorBarPosition.X;
    float BarY = HUDSettings.ArmorBarPosition.Y;
    
    // Calculate fill width
    float FillWidth = BarWidth * ArmorPercentage;
    
    // Draw background
    FCanvasBoxItem BackgroundBox(FVector2D(BarX, BarY), FVector2D(BarWidth, BarHeight));
    BackgroundBox.SetColor(HUDSettings.Colors.Background);
    Canvas->DrawItem(BackgroundBox);
    
    // Draw fill
    FCanvasBoxItem FillBox(FVector2D(BarX, BarY), FVector2D(FillWidth, BarHeight));
    FillBox.SetColor(HUDSettings.Colors.Secondary);
    Canvas->DrawItem(FillBox);
    
    // Draw border
    FCanvasBoxItem BorderBox(FVector2D(BarX, BarY), FVector2D(BarWidth, BarHeight));
    BorderBox.SetColor(HUDSettings.Colors.Primary);
    BorderBox.LineThickness = 1.0f;
    Canvas->DrawItem(BorderBox);
    
    // Draw armor text
    FString ArmorText = FString::Printf(TEXT("%d/%d"), FMath::RoundToInt(CurrentArmor), FMath::RoundToInt(MaxArmor));
    FCanvasTextItem ArmorTextItem(FVector2D(BarX + BarWidth + 10.0f, BarY), FText::FromString(ArmorText), GEngine->GetSmallFont(), FLinearColor::White);
    ArmorTextItem.Scale = FVector2D(0.8f, 0.8f);
    Canvas->DrawItem(ArmorTextItem);
}

void ABlackRemasteredHUD::DrawAmmoCounter()
{
    if (!Canvas)
    {
        return;
    }
    
    // Calculate position (center of screen)
    FVector2D ScreenSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ScreenSize);
    }
    else
    {
        ScreenSize = FVector2D(1920.0f, 1080.0f);
    }
    
    FVector2D Position = FVector2D(ScreenSize.X * 0.5f, ScreenSize.Y - 50.0f);
    
    // Draw ammo text
    FString AmmoText = FString::Printf(TEXT("%d | %d"), CurrentAmmo, ReserveAmmo);
    FCanvasTextItem AmmoTextItem(Position, FText::FromString(AmmoText), GEngine->GetMediumFont(), FLinearColor::White);
    AmmoTextItem.Scale = FVector2D(1.2f, 1.2f);
    AmmoTextItem.bCentreX = true;
    AmmoTextItem.bCentreY = true;
    Canvas->DrawItem(AmmoTextItem);
}

void ABlackRemasteredHUD::DrawWeaponIcon()
{
    // This would draw the weapon icon
    // In a real implementation, you would have a texture for each weapon type
    // For now, we'll just draw the weapon name
    
    if (!Canvas)
    {
        return;
    }
    
    // Calculate position (center of screen, above ammo counter)
    FVector2D ScreenSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ScreenSize);
    }
    else
    {
        ScreenSize = FVector2D(1920.0f, 1080.0f);
    }
    
    FVector2D Position = FVector2D(ScreenSize.X * 0.5f, ScreenSize.Y - 80.0f);
    
    // Draw weapon name
    FString WeaponName = UEnum::GetUserFriendlyName(CurrentWeaponType);
    FCanvasTextItem WeaponTextItem(Position, FText::FromString(WeaponName), GEngine->GetSmallFont(), FLinearColor::White);
    WeaponTextItem.Scale = FVector2D(1.0f, 1.0f);
    WeaponTextItem.bCentreX = true;
    WeaponTextItem.bCentreY = true;
    Canvas->DrawItem(WeaponTextItem);
}

void ABlackRemasteredHUD::DrawCrosshair()
{
    if (!Canvas)
    {
        return;
    }
    
    // Calculate center of screen
    FVector2D ScreenSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ScreenSize);
    }
    else
    {
        ScreenSize = FVector2D(1920.0f, 1080.0f);
    }
    
    FVector2D Center = ScreenSize * 0.5f;
    
    // Draw crosshair based on weapon type
    float CrosshairSize = HUDSettings.CrosshairSize;
    FLinearColor CrosshairColor = FLinearColor::White;
    
    switch (CurrentWeaponType)
    {
        case WT_Pistol:
            // Small crosshair
            CrosshairSize = 8.0f;
            CrosshairColor = FLinearColor(1.0f, 1.0f, 0.5f);
            break;
            
        case WT_Shotgun:
            // Large crosshair with spread indicator
            CrosshairSize = 12.0f;
            CrosshairColor = FLinearColor(1.0f, 0.5f, 0.0f);
            break;
            
        case WT_SMG:
            // Medium crosshair
            CrosshairSize = 10.0f;
            CrosshairColor = FLinearColor(0.5f, 1.0f, 0.5f);
            break;
            
        case WT_AssaultRifle:
            // Standard crosshair
            CrosshairSize = 10.0f;
            CrosshairColor = FLinearColor::White;
            break;
            
        case WT_MachineGun:
            // Large crosshair
            CrosshairSize = 14.0f;
            CrosshairColor = FLinearColor(1.0f, 0.5f, 0.0f);
            break;
            
        case WT_SniperRifle:
            // Precision crosshair
            CrosshairSize = 6.0f;
            CrosshairColor = FLinearColor(0.5f, 1.0f, 0.5f);
            break;
            
        case WT_Heavy:
            // Very large crosshair
            CrosshairSize = 16.0f;
            CrosshairColor = FLinearColor(1.0f, 0.3f, 0.3f);
            break;
            
        default:
            CrosshairSize = 10.0f;
            CrosshairColor = FLinearColor::White;
            break;
    }
    
    // Draw horizontal line
    FCanvasLineItem HorizontalLine(FVector2D(Center.X - CrosshairSize, Center.Y), FVector2D(Center.X + CrosshairSize, Center.Y));
    HorizontalLine.SetColor(CrosshairColor);
    HorizontalLine.LineThickness = 1.0f;
    Canvas->DrawItem(HorizontalLine);
    
    // Draw vertical line
    FCanvasLineItem VerticalLine(FVector2D(Center.X, Center.Y - CrosshairSize), FVector2D(Center.X, Center.Y + CrosshairSize));
    VerticalLine.SetColor(CrosshairColor);
    VerticalLine.LineThickness = 1.0f;
    Canvas->DrawItem(VerticalLine);
}

void ABlackRemasteredHUD::DrawStaminaBar()
{
    if (!Canvas || MaxStamina <= 0)
    {
        return;
    }
    
    // Calculate stamina percentage
    float StaminaPercentage = CurrentStamina / MaxStamina;
    
    // Calculate bar dimensions
    float BarWidth = 200.0f;
    float BarHeight = 10.0f;
    float BarX = HUDSettings.HealthBarPosition.X;
    float BarY = HUDSettings.HealthBarPosition.Y + HUDSettings.HealthBarHeight + 25.0f;
    
    // Calculate fill width
    float FillWidth = BarWidth * StaminaPercentage;
    
    // Draw background
    FCanvasBoxItem BackgroundBox(FVector2D(BarX, BarY), FVector2D(BarWidth, BarHeight));
    BackgroundBox.SetColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
    Canvas->DrawItem(BackgroundBox);
    
    // Draw fill
    FCanvasBoxItem FillBox(FVector2D(BarX, BarY), FVector2D(FillWidth, BarHeight));
    FillBox.SetColor(FLinearColor(0.3f, 0.8f, 0.3f, 1.0f));
    Canvas->DrawItem(FillBox);
    
    // Draw border
    FCanvasBoxItem BorderBox(FVector2D(BarX, BarY), FVector2D(BarWidth, BarHeight));
    BorderBox.SetColor(FLinearColor::White);
    BorderBox.LineThickness = 1.0f;
    Canvas->DrawItem(BorderBox);
}

void ABlackRemasteredHUD::DrawMiniMap()
{
    // Mini-map would be drawn by a widget
    // This is a placeholder for the canvas-based mini-map
}

void ABlackRemasteredHUD::DrawCompass()
{
    if (!Canvas)
    {
        return;
    }
    
    // Calculate position (top center)
    FVector2D ScreenSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ScreenSize);
    }
    else
    {
        ScreenSize = FVector2D(1920.0f, 1080.0f);
    }
    
    FVector2D Position = FVector2D(ScreenSize.X * 0.5f, 50.0f);
    
    // Draw compass directions
    FString CompassText = TEXT("N");
    FCanvasTextItem CompassTextItem(Position, FText::FromString(CompassText), GEngine->GetSmallFont(), FLinearColor::White);
    CompassTextItem.Scale = FVector2D(1.0f, 1.0f);
    CompassTextItem.bCentreX = true;
    CompassTextItem.bCentreY = true;
    Canvas->DrawItem(CompassTextItem);
}

void ABlackRemasteredHUD::DrawObjectiveMarker()
{
    // Objective marker would be drawn at the objective location
    // This requires world-to-screen conversion
}

void ABlackRemasteredHUD::DrawDamageIndicator()
{
    if (!Canvas || !bShowDamageIndicator)
    {
        return;
    }
    
    // Calculate screen size
    FVector2D ScreenSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ScreenSize);
    }
    else
    {
        ScreenSize = FVector2D(1920.0f, 1080.0f);
    }
    
    // Calculate damage indicator position (edge of screen in damage direction)
    FVector2D ScreenCenter = ScreenSize * 0.5f;
    FVector2D IndicatorPosition = ScreenCenter + (DamageIndicatorDirection * 200.0f * DamageIndicatorIntensity);
    
    // Clamp to screen edges
    IndicatorPosition.X = FMath::Clamp(IndicatorPosition.X, 10.0f, ScreenSize.X - 10.0f);
    IndicatorPosition.Y = FMath::Clamp(IndicatorPosition.Y, 10.0f, ScreenSize.Y - 10.0f);
    
    // Draw damage indicator
    FCanvasBoxItem IndicatorBox(FVector2D(IndicatorPosition.X - 10.0f, IndicatorPosition.Y - 10.0f), FVector2D(20.0f, 20.0f));
    IndicatorBox.SetColor(FLinearColor(1.0f, 0.0f, 0.0f, DamageIndicatorIntensity));
    Canvas->DrawItem(IndicatorBox);
    
    // Draw arrow pointing towards damage source
    FCanvasLineItem ArrowLine1(IndicatorPosition, IndicatorPosition + DamageIndicatorDirection * 20.0f);
    ArrowLine1.SetColor(FLinearColor(1.0f, 0.0f, 0.0f, DamageIndicatorIntensity));
    ArrowLine1.LineThickness = 2.0f;
    Canvas->DrawItem(ArrowLine1);
    
    FCanvasLineItem ArrowLine2(IndicatorPosition, IndicatorPosition + DamageIndicatorDirection.Rotate(45.0f) * 10.0f);
    ArrowLine2.SetColor(FLinearColor(1.0f, 0.0f, 0.0f, DamageIndicatorIntensity));
    ArrowLine2.LineThickness = 2.0f;
    Canvas->DrawItem(ArrowLine2);
    
    FCanvasLineItem ArrowLine3(IndicatorPosition, IndicatorPosition + DamageIndicatorDirection.Rotate(-45.0f) * 10.0f);
    ArrowLine3.SetColor(FLinearColor(1.0f, 0.0f, 0.0f, DamageIndicatorIntensity));
    ArrowLine3.LineThickness = 2.0f;
    Canvas->DrawItem(ArrowLine3);
}

void ABlackRemasteredHUD::DrawMessageLog()
{
    if (!Canvas || MessageLog.Num() == 0)
    {
        return;
    }
    
    // Calculate position (bottom center)
    FVector2D ScreenSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ScreenSize);
    }
    else
    {
        ScreenSize = FVector2D(1920.0f, 1080.0f);
    }
    
    float StartY = ScreenSize.Y - 150.0f;
    float LineHeight = 20.0f;
    float CurrentY = StartY;
    
    // Draw messages from bottom to top
    for (int32 i = MessageLog.Num() - 1; i >= 0; i--)
    {
        // Calculate alpha based on timer
        float Alpha = 1.0f;
        if (MessageTimers[i] > HUDSettings.MessageLogDuration * 0.5f)
        {
            Alpha = 1.0f - ((MessageTimers[i] - HUDSettings.MessageLogDuration * 0.5f) / (HUDSettings.MessageLogDuration * 0.5f));
        }
        
        FLinearColor MessageColor = MessageColors[i];
        MessageColor.A = Alpha;
        
        // Draw message
        FCanvasTextItem MessageItem(FVector2D(ScreenSize.X * 0.5f, CurrentY), MessageLog[i], GEngine->GetSmallFont(), MessageColor);
        MessageItem.Scale = FVector2D(0.8f, 0.8f);
        MessageItem.bCentreX = true;
        MessageItem.bCentreY = true;
        Canvas->DrawItem(MessageItem);
        
        CurrentY -= LineHeight;
    }
}

void ABlackRemasteredHUD::DrawInteractionPrompt()
{
    if (!Canvas || !bShowInteractionPrompt || InteractionPrompt.IsEmpty())
    {
        return;
    }
    
    // Calculate position (center of screen)
    FVector2D ScreenSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ScreenSize);
    }
    else
    {
        ScreenSize = FVector2D(1920.0f, 1080.0f);
    }
    
    FVector2D Position = FVector2D(ScreenSize.X * 0.5f, ScreenSize.Y * 0.7f);
    
    // Draw background
    FCanvasBoxItem BackgroundBox(FVector2D(Position.X - 100.0f, Position.Y - 15.0f), FVector2D(200.0f, 30.0f));
    BackgroundBox.SetColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.7f));
    Canvas->DrawItem(BackgroundBox);
    
    // Draw prompt text
    FCanvasTextItem PromptItem(Position, InteractionPrompt, GEngine->GetMediumFont(), FLinearColor::White);
    PromptItem.Scale = FVector2D(1.0f, 1.0f);
    PromptItem.bCentreX = true;
    PromptItem.bCentreY = true;
    Canvas->DrawItem(PromptItem);
}

FVector2D ABlackRemasteredHUD::GetScreenPosition(FVector WorldPosition) const
{
    if (!PlayerController || !PlayerController->PlayerCameraManager)
    {
        return FVector2D::ZeroVector;
    }
    
    // Project world position to screen
    FVector2D ScreenPosition;
    bool bSuccess = PlayerController->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition);
    
    if (!bSuccess)
    {
        return FVector2D::ZeroVector;
    }
    
    return ScreenPosition;
}

void ABlackRemasteredHUD::UpdateHUDData()
{
    if (!PlayerController)
    {
        return;
    }
    
    ABlackRemasteredCharacter* PlayerCharacter = GetPlayerCharacter();
    if (!PlayerCharacter)
    {
        return;
    }
    
    // Update health
    if (UBlackRemasteredHealthComponent* HealthComponent = PlayerCharacter->GetHealthComponent())
    {
        UpdateHealth(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
        UpdateArmor(HealthComponent->GetCurrentArmor(), HealthComponent->GetMaxArmor());
    }
    
    // Update ammo
    if (UBlackRemasteredWeaponComponent* WeaponComponent = PlayerCharacter->GetWeaponComponent())
    {
        if (ABlackRemasteredWeapon* CurrentWeapon = WeaponComponent->GetCurrentWeapon())
        {
            UpdateAmmo(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetReserveAmmo(), CurrentWeapon->GetMagazineCapacity());
            UpdateWeapon(CurrentWeapon->GetWeaponType(), WeaponComponent->GetCurrentWeaponIndex());
        }
    }
    
    // Update stamina
    // This would be updated by the character's stamina system
}
