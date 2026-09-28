#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BRPrototypeHUD.generated.h"

UCLASS()
class BLACKPROTOTYPE_API ABRPrototypeHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
};
