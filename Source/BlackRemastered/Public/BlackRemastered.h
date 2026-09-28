#pragma once

#include "CoreMinimal.h"

// Game version
#define BLACKREMASTERED_VERSION 1

// Debug macros
#if DEBUG
#define BR_LOG(Format, ...) UE_LOG(LogTemp, Log, Format, ##__VA_ARGS__)
#define BR_LOG_WARNING(Format, ...) UE_LOG(LogTemp, Warning, Format, ##__VA_ARGS__)
#define BR_LOG_ERROR(Format, ...) UE_LOG(LogTemp, Error, Format, ##__VA_ARGS__)
#else
#define BR_LOG(Format, ...)
#define BR_LOG_WARNING(Format, ...)
#define BR_LOG_ERROR(Format, ...)
#endif

// Gameplay constants
namespace BlackRemasteredConstants
{
    // Player constants
    const float DEFAULT_WALK_SPEED = 200.0f;
    const float DEFAULT_RUN_SPEED = 300.0f;
    const float DEFAULT_SPRINT_SPEED = 400.0f;
    const float DEFAULT_CROUCH_SPEED = 150.0f;
    const float DEFAULT_PRONE_SPEED = 100.0f;
    const float DEFAULT_SLIDE_SPEED = 500.0f;
    const float DEFAULT_JUMP_VELOCITY = 600.0f;
    const float DEFAULT_GRAVITY_SCALE = 1.0f;
    
    // Camera constants
    const float DEFAULT_CAMERA_FOV = 90.0f;
    const float DEFAULT_AIM_FOV = 60.0f;
    const float DEFAULT_CAMERA_LAG_SPEED = 10.0f;
    const float DEFAULT_CAMERA_LAG_MAX_DISTANCE = 100.0f;
    
    // Health constants
    const float DEFAULT_MAX_HEALTH = 100.0f;
    const float DEFAULT_MAX_ARMOR = 100.0f;
    const float DEFAULT_HEALTH_REGEN_RATE = 20.0f;
    const float DEFAULT_HEALTH_REGEN_DELAY = 1.0f;
    
    // Stamina constants
    const float DEFAULT_MAX_STAMINA = 100.0f;
    const float DEFAULT_STAMINA_REGEN_RATE = 20.0f;
    const float DEFAULT_STAMINA_REGEN_DELAY = 1.0f;
    const float DEFAULT_SPRINT_STAMINA_COST = 10.0f;
    const float DEFAULT_SLIDE_STAMINA_COST = 20.0f;
    
    // Weapon constants
    const int32 DEFAULT_AMMO_CAPACITY = 30;
    const int32 DEFAULT_MAX_AMMO = 90;
    const float DEFAULT_FIRE_RATE = 0.1f;
    const float DEFAULT_RELOAD_TIME = 2.0f;
    const float DEFAULT_DAMAGE = 25.0f;
    
    // Destruction constants
    const float DESTRUCTION_DAMAGE_THRESHOLD = 10.0f;
    const float DESTRUCTION_RADIUS = 500.0f;
    const int32 MAX_DEBRIS_COUNT = 100;
    
    // AI constants
    const float AI_PERCEPTION_RADIUS = 2000.0f;
    const float AI_HEARING_RADIUS = 1000.0f;
    const float AI_VISION_ANGLE = 180.0f;
    
    // Physics constants
    const float GRAVITY_ACCELERATION = 980.0f;
    const float TERMINAL_VELOCITY = 3000.0f;
}

// Utility functions
namespace BlackRemasteredUtils
{
    // Math utilities
    template<typename T>
    T Clamp(T Value, T Min, T Max)
    {
        return FMath::Clamp(Value, Min, Max);
    }
    
    template<typename T>
    T Lerp(T A, T B, float Alpha)
    {
        return FMath::Lerp(A, B, Alpha);
    }
    
    // Random utilities
    float RandomFloat(float Min, float Max);
    int32 RandomInt(int32 Min, int32 Max);
    bool RandomBool(float Probability = 0.5f);
    
    // String utilities
    FString GetWeaponName(EWeaponType WeaponType);
    FString GetEnemyName(EEnemyType EnemyType);
    
    // Damage utilities
    float CalculateDamage(float BaseDamage, float Distance, float Armor, EWeaponType WeaponType);
    float CalculateHeadshotDamage(float BaseDamage, EWeaponType WeaponType);
    
    // Physics utilities
    bool TraceForObjects(UWorld* World, const FVector& Start, const FVector& End, FHitResult& HitResult, ECollisionChannel Channel);
    bool SphereOverlap(UWorld* World, const FVector& Center, float Radius, TArray<AActor*>& OverlappingActors, TSubclassOf<AActor> ClassFilter);
    
    // Debug utilities
    void DrawDebugLine(UWorld* World, const FVector& Start, const FVector& End, FColor Color, float Duration = 0.1f);
    void DrawDebugSphere(UWorld* World, const FVector& Center, float Radius, FColor Color, float Duration = 0.1f);
    void DrawDebugBox(UWorld* World, const FVector& Center, const FVector& Extent, FColor Color, float Duration = 0.1f);
}

// Enums
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

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    WT_Pistol UMETA(DisplayName = "Pistol"),
    WT_Shotgun UMETA(DisplayName = "Shotgun"),
    WT_SMG UMETA(DisplayName = "SMG"),
    WT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
    WT_MachineGun UMETA(DisplayName = "Machine Gun"),
    WT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
    WT_Heavy UMETA(DisplayName = "Heavy Weapon"),
    WT_Melee UMETA(DisplayName = "Melee"),
    WT_Grenade UMETA(DisplayName = "Grenade"),
    WT_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
    WFM_SemiAuto UMETA(DisplayName = "Semi-Auto"),
    WFM_FullAuto UMETA(DisplayName = "Full-Auto"),
    WFM_Burst UMETA(DisplayName = "Burst"),
    WFM_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
    AT_Standard UMETA(DisplayName = "Standard"),
    AT_ArmorPiercing UMETA(DisplayName = "Armor Piercing"),
    AT_Incendiary UMETA(DisplayName = "Incendiary"),
    AT_Explosive UMETA(DisplayName = "Explosive"),
    AT_HollowPoint UMETA(DisplayName = "Hollow Point"),
    AT_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
    ET_Grunt UMETA(DisplayName = "Grunt"),
    ET_Soldier UMETA(DisplayName = "Soldier"),
    ET_Veteran UMETA(DisplayName = "Veteran"),
    ET_Sniper UMETA(DisplayName = "Sniper"),
    ET_Shotgunner UMETA(DisplayName = "Shotgunner"),
    ET_MachineGunner UMETA(DisplayName = "Machine Gunner"),
    ET_Grenadier UMETA(DisplayName = "Grenadier"),
    ET_Engineer UMETA(DisplayName = "Engineer"),
    ET_Heavy UMETA(DisplayName = "Heavy"),
    ET_RPG UMETA(DisplayName = "RPG Trooper"),
    ET_Shield UMETA(DisplayName = "Shield Bearer"),
    ET_Commander UMETA(DisplayName = "Commander"),
    ET_Spetsnaz UMETA(DisplayName = "Spetsnaz"),
    ET_BlackOps UMETA(DisplayName = "Black Ops"),
    ET_Boss UMETA(DisplayName = "Boss"),
    ET_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    ES_Idle UMETA(DisplayName = "Idle"),
    ES_Patrol UMETA(DisplayName = "Patrol"),
    ES_Alert UMETA(DisplayName = "Alert"),
    ES_Search UMETA(DisplayName = "Search"),
    ES_Engage UMETA(DisplayName = "Engage"),
    ES_Suppress UMETA(DisplayName = "Suppress"),
    ES_Retreat UMETA(DisplayName = "Retreat"),
    ES_Regroup UMETA(DisplayName = "Regroup"),
    ES_Dead UMETA(DisplayName = "Dead"),
    ES_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDestructionTier : uint8
{
    DT_Full UMETA(DisplayName = "Full Destruction"),
    DT_Partial UMETA(DisplayName = "Partial Destruction"),
    DT_Surface UMETA(DisplayName = "Surface Damage"),
    DT_Indestructible UMETA(DisplayName = "Indestructible"),
    DT_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
    DT_Ballistic UMETA(DisplayName = "Ballistic"),
    DT_Explosive UMETA(DisplayName = "Explosive"),
    DT_Fire UMETA(DisplayName = "Fire"),
    DT_Melee UMETA(DisplayName = "Melee"),
    DT_Fall UMETA(DisplayName = "Fall"),
    DT_MAX UMETA(Hidden)
};

// Forward declarations
class ABlackRemasteredGameMode;
class ABlackRemasteredGameState;
class ABlackRemasteredPlayerState;
class ABlackRemasteredCharacter;
class ABlackRemasteredPlayerController;
class ABlackRemasteredAIController;
class ABlackRemasteredEnemyCharacter;
class ABlackRemasteredWeapon;
class ABlackRemasteredProjectile;
class ABlackRemasteredGrenade;
class ABlackRemasteredDestructibleActor;

class UBlackRemasteredWeaponComponent;
class UBlackRemasteredHealthComponent;
class UBlackRemasteredMovementComponent;
class UBlackRemasteredInventoryComponent;
class UBlackRemasteredInteractionComponent;
class UBlackRemasteredDestructionComponent;
class UBlackRemasteredAIControllerComponent;
class UBlackRemasteredHUD;
class UBlackRemasteredSaveSystem;
class UBlackRemasteredProgressionSystem;
class UBlackRemasteredAudioSystem;
class UBlackRemasteredUIManager;
