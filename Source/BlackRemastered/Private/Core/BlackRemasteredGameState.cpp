#include "BlackRemasteredGameState.h"

ABlackRemasteredGameState::ABlackRemasteredGameState()
    : Super()
{
    CurrentLevelIndex = 0;
    CurrentMissionIndex = 0;
    TotalKills = 0;
    TotalDeaths = 0;
    TotalShotsFired = 0;
    TotalShotsHit = 0;
    TotalHeadshots = 0;
    TotalDestruction = 0;
    TotalPlayTime = 0.0f;
    TotalExplosions = 0;
    TotalGrenadesThrown = 0;
    bLevelCompleted = false;
    bMissionCompleted = false;
    bGameCompleted = false;
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

float ABlackRemasteredGameState::GetHeadshotPercentage() const
{
    if (TotalShotsHit <= 0)
    {
        return 0.0f;
    }
    return static_cast<float>(TotalHeadshots) / static_cast<float>(TotalShotsHit) * 100.0f;
}

void ABlackRemasteredGameState::AddKill()
{
    TotalKills++;
}

void ABlackRemasteredGameState::AddDeath()
{
    TotalDeaths++;
}

void ABlackRemasteredGameState::AddShot(bool bHit, bool bHeadshot)
{
    TotalShotsFired++;
    if (bHit)
    {
        TotalShotsHit++;
        if (bHeadshot)
        {
            TotalHeadshots++;
        }
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

void ABlackRemasteredGameState::AddExplosion()
{
    TotalExplosions++;
}

void ABlackRemasteredGameState::AddGrenade()
{
    TotalGrenadesThrown++;
}

void ABlackRemasteredGameState::ResetStatistics()
{
    TotalKills = 0;
    TotalDeaths = 0;
    TotalShotsFired = 0;
    TotalShotsHit = 0;
    TotalHeadshots = 0;
    TotalDestruction = 0;
    TotalPlayTime = 0.0f;
    TotalExplosions = 0;
    TotalGrenadesThrown = 0;
}

void ABlackRemasteredGameState::SetLevelCompleted(bool bCompleted)
{
    bLevelCompleted = bCompleted;
}

void ABlackRemasteredGameState::SetMissionCompleted(bool bCompleted)
{
    bMissionCompleted = bCompleted;
}

void ABlackRemasteredGameState::SetGameCompleted(bool bCompleted)
{
    bGameCompleted = bCompleted;
}

void ABlackRemasteredGameState::OnPlayTimeTimer()
{
    AddPlayTime(1.0f);
}
