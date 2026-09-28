#include "BRPrototypeHUD.h"
#include "BRPrototypeGameMode.h"
#include "BRPrototypePlayer.h"
#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void ABRPrototypeHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas)
    {
        return;
    }

    const float CenterX = Canvas->ClipX * 0.5f;
    const float CenterY = Canvas->ClipY * 0.5f;
    DrawLine(CenterX - 12, CenterY, CenterX - 4, CenterY, FLinearColor::White, 2.0f);
    DrawLine(CenterX + 4, CenterY, CenterX + 12, CenterY, FLinearColor::White, 2.0f);
    DrawLine(CenterX, CenterY - 12, CenterX, CenterY - 4, FLinearColor::White, 2.0f);
    DrawLine(CenterX, CenterY + 4, CenterX, CenterY + 12, FLinearColor::White, 2.0f);

    const ABRPrototypePlayer* Player = Cast<ABRPrototypePlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
    const ABRPrototypeGameMode* Mode = GetWorld()->GetAuthGameMode<ABRPrototypeGameMode>();

    DrawText(TEXT("BLACK | TESTARENA"), FLinearColor::White, 24, 20);
    DrawText(TEXT("WASD: bewegen  |  Maus: zielen/schiessen  |  Shift: sprinten"),
        FLinearColor::White, 24, Canvas->ClipY - 72);
    DrawText(TEXT("Leertaste: springen  |  R: nachladen  |  F5: Neustart  |  Esc: Pause"),
        FLinearColor::White, 24, Canvas->ClipY - 45);

    if (Player)
    {
        DrawText(FString::Printf(TEXT("LEBEN: %.0f"), Player->GetHealth()),
            FLinearColor::White, 24, 53);
        DrawText(FString::Printf(TEXT("MUNITION: %d / %d%s"), Player->GetAmmoInMagazine(),
            Player->GetReserveAmmo(), Player->IsReloading() ? TEXT("  NACHLADEN") : TEXT("")),
            FLinearColor::White, 24, 80);
    }

    if (Mode)
    {
        DrawText(FString::Printf(TEXT("GEGNER: %d"), Mode->GetRemainingEnemies()),
            FLinearColor::White, 24, 107);
        if (Mode->GetRemainingEnemies() == 0)
        {
            DrawText(TEXT("ARENA GESCHAFFT - F5 fuer neue Runde"),
                FLinearColor::Yellow, CenterX - 170, CenterY + 45);
        }
    }

    if (UGameplayStatics::IsGamePaused(this))
    {
        DrawText(TEXT("PAUSE - Esc zum Fortsetzen"),
            FLinearColor::Yellow, CenterX - 125, CenterY - 60);
    }
}
