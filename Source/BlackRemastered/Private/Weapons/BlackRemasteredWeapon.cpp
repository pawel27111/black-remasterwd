#include "BlackRemasteredWeapon.h"
#include "BlackRemasteredCharacter.h"
#include "BlackRemasteredProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Camera/CameraShake.h"
#include "Core/BlackRemasteredGameState.h"

ABlackRemasteredWeapon::ABlackRemasteredWeapon()
    : Super()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Create mesh
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
    
    // Initialize state
    CurrentState = WS_Idle;
    CurrentFireMode = WFM_SemiAuto;
    CurrentHeat = 0.0f;
    BurstShotsFired = 0;
    LastFireTime = 0.0f;
    CurrentVerticalRecoil = 0.0f;
    CurrentHorizontalRecoil = 0.0f;
    CurrentSpread = 0.0f;
    bFirstShotAccuracy = true;
    
    // Initialize weapon data with defaults
    WeaponData.WeaponName = FName("DefaultWeapon");
    WeaponData.WeaponType = WT_AssaultRifle;
    WeaponData.MagazineCapacity = 30;
    WeaponData.MaxAmmo = 90;
    WeaponData.CurrentAmmo = 30;
    WeaponData.ReserveAmmo = 60;
    WeaponData.AmmoType = AT_Standard;
    WeaponData.BaseDamage = 25.0f;
    WeaponData.HeadshotMultiplier = 2.0f;
    WeaponData.ArmorPenetration = 0.5f;
    WeaponData.FireRate = 0.1f;
    WeaponData.BurstFireRate = 0.08f;
    WeaponData.BurstCount = 3;
    WeaponData.HipFireSpread = 5.0f;
    WeaponData.ADSSpread = 1.0f;
    WeaponData.MovementSpread = 3.0f;
    WeaponData.JumpingSpread = 10.0f;
    WeaponData.VerticalRecoil = 2.0f;
    WeaponData.HorizontalRecoil = 1.0f;
    WeaponData.FirstShotAccuracyBonus = 0.5f;
    WeaponData.RecoilRecoverySpeed = 10.0f;
    WeaponData.ReloadTime = 2.0f;
    WeaponData.TacticalReloadTime = 1.5f;
    WeaponData.bInterruptibleReload = true;
    WeaponData.bCanOverheat = false;
    WeaponData.OverheatThreshold = 50.0f;
    WeaponData.OverheatCooldown = 3.0f;
    WeaponData.HeatPerShot = 1.0f;
    WeaponData.HeatCooldownRate = 5.0f;
    WeaponData.bCanJam = false;
    WeaponData.JamProbability = 0.01f;
    WeaponData.JamClearTime = 2.0f;
    WeaponData.MaxRange = 10000.0f;
    WeaponData.EffectiveRange = 5000.0f;
    WeaponData.DamageFalloffStart = 2000.0f;
    WeaponData.DamageFalloffEnd = 5000.0f;
    WeaponData.bUseProjectile = false;
    WeaponData.ProjectileSpeed = 5000.0f;
    WeaponData.ProjectileGravityScale = 0.0f;
    WeaponData.CameraShakeScale = 1.0f;
    
    // Initialize attachments
    WeaponData.AvailableAttachments = { WA_RedDot, WA_Suppressor, WA_ExtendedMag, WA_Foregrip };
    
    // Set default fire mode
    CurrentFireMode = WeaponData.DefaultFireMode;
    
    // Set owner
    OwnerCharacter = nullptr;
}

void ABlackRemasteredWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ABlackRemasteredWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update weapon state
    switch (CurrentState)
    {
        case WS_Firing:
            // Handle automatic fire
            if (CurrentFireMode == WFM_FullAuto || CurrentFireMode == WFM_Burst)
            {
                // Continue firing while trigger is held
                // This is handled by the Fire() function being called repeatedly
            }
            break;
            
        case WS_Overheated:
            // Cool down
            CoolDown(DeltaTime);
            break;
            
        case WS_Jammed:
            // Wait for jam to be cleared
            break;
            
        default:
            break;
    }
    
    // Update recoil recovery
    if (CurrentVerticalRecoil > 0)
    {
        CurrentVerticalRecoil = FMath::Max(CurrentVerticalRecoil - WeaponData.RecoilRecoverySpeed * DeltaTime, 0.0f);
    }
    
    if (CurrentHorizontalRecoil > 0)
    {
        CurrentHorizontalRecoil = FMath::Max(CurrentHorizontalRecoil - WeaponData.RecoilRecoverySpeed * DeltaTime, 0.0f);
    }
    
    // Update spread
    CurrentSpread = FMath::Max(CurrentSpread - WeaponData.RecoilRecoverySpeed * DeltaTime, 0.0f);
    
    // Reset first shot accuracy if not firing
    if (CurrentState != WS_Firing)
    {
        bFirstShotAccuracy = true;
    }
}

void ABlackRemasteredWeapon::Initialize(ABlackRemasteredCharacter* Owner, const FWeaponData& Data)
{
    OwnerCharacter = Owner;
    WeaponData = Data;
    
    // Initialize current fire mode
    CurrentFireMode = WeaponData.DefaultFireMode;
    
    // Initialize ammo
    WeaponData.CurrentAmmo = FMath::Min(WeaponData.CurrentAmmo, WeaponData.MagazineCapacity);
    WeaponData.ReserveAmmo = FMath::Min(WeaponData.ReserveAmmo, WeaponData.MaxAmmo);
    
    // Set state
    CurrentState = WS_Idle;
    CurrentHeat = 0.0f;
    BurstShotsFired = 0;
    LastFireTime = 0.0f;
    CurrentVerticalRecoil = 0.0f;
    CurrentHorizontalRecoil = 0.0f;
    CurrentSpread = 0.0f;
    bFirstShotAccuracy = true;
    
    // Set up mesh
    if (WeaponData.Mesh)
    {
        WeaponMesh->SetSkeletalMesh(WeaponData.Mesh);
    }
}

void ABlackRemasteredWeapon::Fire()
{
    if (!CanFire())
    {
        return;
    }
    
    // Check if we can fire based on fire rate
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastFireTime < WeaponData.FireRate)
    {
        return;
    }
    
    // Check for jam
    CheckForJam();
    if (CurrentState == WS_Jammed)
    {
        return;
    }
    
    // Check for overheat
    CheckForOverheat();
    if (CurrentState == WS_Overheated)
    {
        return;
    }
    
    // Check ammo
    if (!HasAmmo())
    {
        // Play dry fire sound
        if (WeaponData.DryFireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData.DryFireSound, GetActorLocation());
        }
        return;
    }
    
    // Set state
    SetWeaponState(WS_Firing);
    
    // Update last fire time
    LastFireTime = CurrentTime;
    
    // Apply spread based on state
    CurrentSpread = 0.0f;
    
    if (OwnerCharacter)
    {
        // Apply movement spread
        if (OwnerCharacter->IsSprinting())
        {
            CurrentSpread += WeaponData.MovementSpread * 2.0f;
        }
        else if (!OwnerCharacter->IsAiming())
        {
            CurrentSpread += WeaponData.HipFireSpread;
        }
        
        if (OwnerCharacter->IsCrouching() || OwnerCharacter->IsProne())
        {
            CurrentSpread *= 0.7f;
        }
        
        if (OwnerCharacter->IsSliding())
        {
            CurrentSpread *= 1.5f;
        }
        
        // Apply first shot accuracy bonus
        if (bFirstShotAccuracy && OwnerCharacter->IsAiming())
        {
            CurrentSpread *= (1.0f - WeaponData.FirstShotAccuracyBonus);
        }
    }
    
    // Apply recoil
    ApplyRecoil();
    
    // Fire weapon
    if (WeaponData.bUseProjectile)
    {
        FireProjectile();
    }
    else
    {
        FireHitscan();
    }
    
    // Play effects
    PlayFireEffects();
    
    // Consume ammo
    ConsumeAmmo();
    
    // Update stats
    WeaponData.ShotsFired++;
    
    // Broadcast fire event
    OnFire.Broadcast();
    
    // Update heat
    if (WeaponData.bCanOverheat)
    {
        CurrentHeat += WeaponData.HeatPerShot;
        CheckForOverheat();
    }
    
    // Handle burst fire
    if (CurrentFireMode == WFM_Burst)
    {
        BurstShotsFired++;
        
        if (BurstShotsFired >= WeaponData.BurstCount)
        {
            // Burst complete, wait for reset
            GetWorld()->GetTimerManager().SetTimer(
                BurstFireTimerHandle,
                this,
                &ABlackRemasteredWeapon::OnBurstFireTimer,
                WeaponData.BurstFireRate * 2.0f,
                false
            );
        }
    }
    
    // Update first shot accuracy
    bFirstShotAccuracy = false;
    
    // Update game state stats
    if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
    {
        GameState->AddShot(false); // Will be updated to true if hit
    }
}

void ABlackRemasteredWeapon::StopFire()
{
    // End burst fire if in progress
    if (CurrentFireMode == WFM_Burst && BurstShotsFired > 0)
    {
        // Reset burst
        BurstShotsFired = 0;
        GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
    }
    
    // Reset first shot accuracy
    bFirstShotAccuracy = true;
}

void ABlackRemasteredWeapon::FireProjectile()
{
    if (!WeaponData.ProjectileClass)
    {
        return;
    }
    
    // Get camera transform
    FVector StartLocation;
    FRotator StartRotation;
    
    if (OwnerCharacter && OwnerCharacter->GetFollowCamera())
    {
        StartLocation = OwnerCharacter->GetFollowCamera()->GetComponentLocation();
        StartRotation = OwnerCharacter->GetFollowCamera()->GetComponentRotation();
    }
    else
    {
        StartLocation = GetActorLocation();
        StartRotation = GetActorRotation();
    }
    
    // Apply spread
    FVector Direction = StartRotation.Vector();
    ApplySpread(Direction);
    
    // Apply recoil
    if (OwnerCharacter && OwnerCharacter->IsAiming())
    {
        // Reduce recoil when aiming
        Direction = Direction.RotateAngleAxis(CurrentVerticalRecoil * 0.5f, StartRotation.RightVector);
        Direction = Direction.RotateAngleAxis(CurrentHorizontalRecoil * 0.5f, StartRotation.UpVector);
    }
    else
    {
        Direction = Direction.RotateAngleAxis(CurrentVerticalRecoil, StartRotation.RightVector);
        Direction = Direction.RotateAngleAxis(CurrentHorizontalRecoil, StartRotation.UpVector);
    }
    
    // Spawn projectile
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = OwnerCharacter;
    SpawnParams.Instigator = OwnerCharacter ? OwnerCharacter->GetInstigator() : nullptr;
    
    ABlackRemasteredProjectile* Projectile = GetWorld()->SpawnActor<ABlackRemasteredProjectile>(
        WeaponData.ProjectileClass,
        StartLocation,
        StartRotation,
        SpawnParams
    );
    
    if (Projectile)
    {
        Projectile->Initialize(this, Direction * WeaponData.ProjectileSpeed);
    }
}

void ABlackRemasteredWeapon::FireHitscan()
{
    // Get camera transform
    FVector StartLocation;
    FRotator StartRotation;
    
    if (OwnerCharacter && OwnerCharacter->GetFollowCamera())
    {
        StartLocation = OwnerCharacter->GetFollowCamera()->GetComponentLocation();
        StartRotation = OwnerCharacter->GetFollowCamera()->GetComponentRotation();
    }
    else
    {
        StartLocation = GetActorLocation();
        StartRotation = GetActorRotation();
    }
    
    // Calculate end location
    FVector EndLocation = StartLocation + (StartRotation.Vector() * WeaponData.MaxRange);
    
    // Apply spread
    FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
    ApplySpread(Direction);
    
    // Apply recoil
    if (OwnerCharacter && OwnerCharacter->IsAiming())
    {
        // Reduce recoil when aiming
        Direction = Direction.RotateAngleAxis(CurrentVerticalRecoil * 0.5f, StartRotation.RightVector);
        Direction = Direction.RotateAngleAxis(CurrentHorizontalRecoil * 0.5f, StartRotation.UpVector);
    }
    else
    {
        Direction = Direction.RotateAngleAxis(CurrentVerticalRecoil, StartRotation.RightVector);
        Direction = Direction.RotateAngleAxis(CurrentHorizontalRecoil, StartRotation.UpVector);
    }
    
    EndLocation = StartLocation + (Direction * WeaponData.MaxRange);
    
    // Perform trace
    FHitResult HitResult = PerformWeaponTrace(StartLocation, EndLocation);
    
    // Process hit
    if (HitResult.IsValidBlockingHit())
    {
        // Calculate damage based on distance
        float Distance = FVector::Distance(StartLocation, HitResult.ImpactPoint);
        float Damage = CalculateDamage(Distance);
        
        // Apply damage to hit actor
        if (HitResult.GetActor())
        {
            UGameplayStatics::ApplyDamage(
                HitResult.GetActor(),
                Damage,
                OwnerCharacter ? OwnerCharacter->GetController() : nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
        
        // Spawn impact effects
        // TODO: Spawn impact particle effects
        
        // Update stats
        WeaponData.ShotsHit++;
        
        // Update game state stats
        if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
        {
            // Check if this was the last shot
            GameState->AddShot(true);
        }
    }
    else
    {
        // Update game state stats
        if (ABlackRemasteredGameState* GameState = GetWorld()->GetGameState<ABlackRemasteredGameState>())
        {
            GameState->AddShot(false);
        }
    }
}

void ABlackRemasteredWeapon::Reload()
{
    if (!CanReload())
    {
        return;
    }
    
    // Check if we have reserve ammo
    if (WeaponData.ReserveAmmo <= 0)
    {
        // Play out of ammo sound
        if (WeaponData.OutOfAmmoSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData.OutOfAmmoSound, GetActorLocation());
        }
        return;
    }
    
    // Check if already full
    if (IsFull())
    {
        return;
    }
    
    // Set state
    SetWeaponState(WS_Reloading);
    
    // Play reload sound
    PlayReloadEffects();
    
    // Start reload timer
    GetWorld()->GetTimerManager().SetTimer(
        ReloadTimerHandle,
        this,
        &ABlackRemasteredWeapon::OnReloadTimer,
        WeaponData.ReloadTime,
        false
    );
    
    // Broadcast reload event
    OnReload.Broadcast();
}

void ABlackRemasteredWeapon::TacticalReload()
{
    if (!CanReload())
    {
        return;
    }
    
    // Check if we have reserve ammo
    if (WeaponData.ReserveAmmo <= 0)
    {
        return;
    }
    
    // Check if already full
    if (IsFull())
    {
        return;
    }
    
    // Set state
    SetWeaponState(WS_Reloading);
    
    // Play tactical reload sound
    if (WeaponData.TacticalReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData.TacticalReloadSound, GetActorLocation());
    }
    
    // Start reload timer
    GetWorld()->GetTimerManager().SetTimer(
        ReloadTimerHandle,
        this,
        &ABlackRemasteredWeapon::OnReloadTimer,
        WeaponData.TacticalReloadTime,
        false
    );
    
    // Broadcast reload event
    OnReload.Broadcast();
}

void ABlackRemasteredWeapon::CancelReload()
{
    if (CurrentState == WS_Reloading && WeaponData.bInterruptibleReload)
    {
        // Clear reload timer
        GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
        
        // Set state back to idle
        SetWeaponState(WS_Idle);
    }
}

void ABlackRemasteredWeapon::CompleteReload()
{
    if (CurrentState != WS_Reloading)
    {
        return;
    }
    
    // Calculate how much ammo to add
    int32 AmmoToAdd = WeaponData.MagazineCapacity - WeaponData.CurrentAmmo;
    int32 AvailableAmmo = FMath::Min(WeaponData.ReserveAmmo, AmmoToAdd);
    
    // Add ammo
    WeaponData.CurrentAmmo += AvailableAmmo;
    WeaponData.ReserveAmmo -= AvailableAmmo;
    
    // Set state
    SetWeaponState(WS_Idle);
    
    // Broadcast reload complete event
    OnReloadComplete.Broadcast();
    
    // Broadcast ammo changed event
    OnAmmoChanged.Broadcast();
    
    // Reset burst fire counter
    BurstShotsFired = 0;
}

void ABlackRemasteredWeapon::Melee()
{
    if (!CanMelee())
    {
        return;
    }
    
    // Set state
    SetWeaponState(WS_Melee);
    
    // Play melee effects
    PlayMeleeEffects();
    
    // Perform melee trace
    FVector StartLocation;
    FRotator StartRotation;
    
    if (OwnerCharacter && OwnerCharacter->GetFollowCamera())
    {
        StartLocation = OwnerCharacter->GetFollowCamera()->GetComponentLocation();
        StartRotation = OwnerCharacter->GetFollowCamera()->GetComponentRotation();
    }
    else
    {
        StartLocation = GetActorLocation();
        StartRotation = GetActorRotation();
    }
    
    FVector EndLocation = StartLocation + (StartRotation.Vector() * 150.0f);
    
    FHitResult HitResult = PerformWeaponTrace(StartLocation, EndLocation);
    
    // Process hit
    if (HitResult.IsValidBlockingHit())
    {
        // Apply melee damage
        if (HitResult.GetActor())
        {
            UGameplayStatics::ApplyDamage(
                HitResult.GetActor(),
                50.0f, // Melee damage
                OwnerCharacter ? OwnerCharacter->GetController() : nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
        
        // Spawn melee impact effects
        // TODO: Spawn melee impact particle effects
    }
    
    // Set state back to idle
    SetWeaponState(WS_Idle);
}

void ABlackRemasteredWeapon::AddAmmo(int32 Amount)
{
    WeaponData.CurrentAmmo = FMath::Min(WeaponData.CurrentAmmo + Amount, WeaponData.MagazineCapacity);
    
    // Broadcast ammo changed event
    OnAmmoChanged.Broadcast();
}

void ABlackRemasteredWeapon::AddReserveAmmo(int32 Amount)
{
    WeaponData.ReserveAmmo = FMath::Min(WeaponData.ReserveAmmo + Amount, WeaponData.MaxAmmo);
    
    // Broadcast ammo changed event
    OnAmmoChanged.Broadcast();
}

void ABlackRemasteredWeapon::SetAmmo(int32 Magazine, int32 Reserve)
{
    WeaponData.CurrentAmmo = FMath::Min(Magazine, WeaponData.MagazineCapacity);
    WeaponData.ReserveAmmo = FMath::Min(Reserve, WeaponData.MaxAmmo);
    
    // Broadcast ammo changed event
    OnAmmoChanged.Broadcast();
}

void ABlackRemasteredWeapon::ToggleFireMode()
{
    if (WeaponData.AvailableFireModes.Num() <= 1)
    {
        return;
    }
    
    // Find current fire mode index
    int32 CurrentIndex = WeaponData.AvailableFireModes.Find(CurrentFireMode);
    if (CurrentIndex == INDEX_NONE)
    {
        return;
    }
    
    // Get next fire mode
    int32 NextIndex = (CurrentIndex + 1) % WeaponData.AvailableFireModes.Num();
    SetFireMode(WeaponData.AvailableFireModes[NextIndex]);
}

void ABlackRemasteredWeapon::SetFireMode(EWeaponFireMode NewFireMode)
{
    if (WeaponData.AvailableFireModes.Contains(NewFireMode))
    {
        CurrentFireMode = NewFireMode;
        
        // Reset burst fire counter
        BurstShotsFired = 0;
        GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
    }
}

void ABlackRemasteredWeapon::AddAttachment(EWeaponAttachment Attachment)
{
    if (!WeaponData.AvailableAttachments.Contains(Attachment))
    {
        return;
    }
    
    if (HasAttachment(Attachment))
    {
        return;
    }
    
    // Add attachment
    WeaponData.AvailableAttachments.Add(Attachment);
    
    // Apply attachment effects
    ApplyAttachmentEffects(Attachment, true);
}

void ABlackRemasteredWeapon::RemoveAttachment(EWeaponAttachment Attachment)
{
    if (!HasAttachment(Attachment))
    {
        return;
    }
    
    // Remove attachment
    WeaponData.AvailableAttachments.Remove(Attachment);
    
    // Remove attachment effects
    ApplyAttachmentEffects(Attachment, false);
}

bool ABlackRemasteredWeapon::HasAttachment(EWeaponAttachment Attachment) const
{
    return WeaponData.AvailableAttachments.Contains(Attachment);
}

void ABlackRemasteredWeapon::Overheat()
{
    if (CurrentState == WS_Overheated)
    {
        return;
    }
    
    SetWeaponState(WS_Overheated);
    
    // Play overheat effects
    PlayOverheatEffects();
    
    // Start cooldown timer
    GetWorld()->GetTimerManager().SetTimer(
        OverheatTimerHandle,
        this,
        &ABlackRemasteredWeapon::OnOverheatTimer,
        WeaponData.OverheatCooldown,
        false
    );
    
    // Broadcast overheat event
    OnOverheat.Broadcast();
}

void ABlackRemasteredWeapon::CoolDown(float DeltaTime)
{
    if (CurrentHeat > 0)
    {
        CurrentHeat = FMath::Max(CurrentHeat - WeaponData.HeatCooldownRate * DeltaTime, 0.0f);
        
        // Check if cooled down enough
        if (CurrentHeat <= WeaponData.OverheatThreshold * 0.5f)
        {
            SetWeaponState(WS_Idle);
        }
    }
}

void ABlackRemasteredWeapon::Jam()
{
    if (CurrentState == WS_Jammed || !WeaponData.bCanJam)
    {
        return;
    }
    
    SetWeaponState(WS_Jammed);
    
    // Play jam effects
    PlayJamEffects();
    
    // Start jam clear timer
    GetWorld()->GetTimerManager().SetTimer(
        JamTimerHandle,
        this,
        &ABlackRemasteredWeapon::OnJamTimer,
        WeaponData.JamClearTime,
        false
    );
    
    // Broadcast jam event
    OnJam.Broadcast();
}

void ABlackRemasteredWeapon::ClearJam()
{
    if (CurrentState != WS_Jammed)
    {
        return;
    }
    
    // Clear jam timer
    GetWorld()->GetTimerManager().ClearTimer(JamTimerHandle);
    
    // Set state back to idle
    SetWeaponState(WS_Idle);
    
    // Reset burst fire counter
    BurstShotsFired = 0;
}

void ABlackRemasteredWeapon::SetWeaponState(EWeaponState NewState)
{
    EWeaponState OldState = CurrentState;
    CurrentState = NewState;
    
    // Broadcast state change
    OnStateChanged.Broadcast();
}

bool ABlackRemasteredWeapon::CanFire() const
{
    if (CurrentState == WS_Reloading || CurrentState == WS_Jammed || CurrentState == WS_Overheated)
    {
        return false;
    }
    
    if (!HasAmmo())
    {
        return false;
    }
    
    return true;
}

bool ABlackRemasteredWeapon::CanReload() const
{
    if (CurrentState == WS_Reloading || CurrentState == WS_Jammed || CurrentState == WS_Overheated)
    {
        return false;
    }
    
    if (IsFull())
    {
        return false;
    }
    
    if (!HasReserveAmmo())
    {
        return false;
    }
    
    return true;
}

bool ABlackRemasteredWeapon::CanMelee() const
{
    if (CurrentState == WS_Reloading || CurrentState == WS_Jammed || CurrentState == WS_Overheated)
    {
        return false;
    }
    
    return true;
}

float ABlackRemasteredWeapon::CalculateDamage(float Distance) const
{
    float Damage = WeaponData.BaseDamage;
    
    // Apply distance falloff
    if (Distance > WeaponData.DamageFalloffStart)
    {
        float FalloffFactor = FMath::Clamp(
            (Distance - WeaponData.DamageFalloffStart) / 
            (WeaponData.DamageFalloffEnd - WeaponData.DamageFalloffStart),
            0.0f,
            1.0f
        );
        Damage *= (1.0f - FalloffFactor * 0.5f);
    }
    
    return Damage;
}

void ABlackRemasteredWeapon::ApplyAttachmentEffects(EWeaponAttachment Attachment, bool bApply)
{
    switch (Attachment)
    {
        case WA_Suppressor:
            if (bApply)
            {
                // Reduce noise
                // TODO: Modify audio properties
            }
            break;
            
        case WA_ExtendedMag:
            if (bApply)
            {
                // Increase magazine capacity
                WeaponData.MagazineCapacity = FMath::RoundToInt(WeaponData.MagazineCapacity * 1.5f);
            }
            else
            {
                // Restore original capacity
                // TODO: Store original capacity
            }
            break;
            
        case WA_Foregrip:
            if (bApply)
            {
                // Reduce recoil
                WeaponData.VerticalRecoil *= 0.7f;
                WeaponData.HorizontalRecoil *= 0.7f;
            }
            else
            {
                // Restore original recoil
                // TODO: Store original recoil
            }
            break;
            
        case WA_RedDot:
        case WA_ACOG:
        case WA_SniperScope:
            // Scope attachments don't affect gameplay directly
            break;
            
        default:
            break;
    }
}

// Timer callbacks
void ABlackRemasteredWeapon::OnFireTimer()
{
    // This is used for automatic fire in full-auto mode
    // But we handle it in the Fire() function instead
}

void ABlackRemasteredWeapon::OnReloadTimer()
{
    CompleteReload();
}

void ABlackRemasteredWeapon::OnOverheatTimer()
{
    // Start cooling down
    CurrentHeat = WeaponData.OverheatThreshold;
}

void ABlackRemasteredWeapon::OnJamTimer()
{
    ClearJam();
}

void ABlackRemasteredWeapon::OnBurstFireTimer()
{
    BurstShotsFired = 0;
}

// Helper functions
void ABlackRemasteredWeapon::PlayFireEffects()
{
    // Play fire sound
    if (WeaponData.FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData.FireSound, GetActorLocation());
    }
    
    // Spawn muzzle flash
    if (WeaponData.MuzzleFlash && !WeaponData.MuzzleFlashSocket.IsNone())
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            WeaponData.MuzzleFlash,
            WeaponMesh->GetSocketLocation(WeaponData.MuzzleFlashSocket),
            WeaponMesh->GetSocketRotation(WeaponData.MuzzleFlashSocket)
        );
    }
    
    // Spawn shell ejection
    if (WeaponData.ShellEjection && !WeaponData.ShellEjectionSocket.IsNone())
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            WeaponData.ShellEjection,
            WeaponMesh->GetSocketLocation(WeaponData.ShellEjectionSocket),
            WeaponMesh->GetSocketRotation(WeaponData.ShellEjectionSocket)
        );
    }
    
    // Play camera shake
    if (WeaponData.FireCameraShake && OwnerCharacter)
    {
        if (APlayerController* PlayerController = OwnerCharacter->GetController<APlayerController>())
        {
            PlayerController->PlayerCameraManager->StartCameraShake(
                WeaponData.FireCameraShake,
                WeaponData.CameraShakeScale
            );
        }
    }
    
    // Cock weapon (for semi-auto)
    if (CurrentFireMode == WFM_SemiAuto && WeaponData.CockSound)
    {
        // Play cock sound on next fire
        // TODO: Implement
    }
}

void ABlackRemasteredWeapon::PlayReloadEffects()
{
    // Play reload sound
    if (WeaponData.ReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData.ReloadSound, GetActorLocation());
    }
    
    // Play reload animation
    if (WeaponData.ReloadAnimation && OwnerCharacter)
    {
        if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
        {
            CharacterMesh->PlayAnimation(WeaponData.ReloadAnimation, false);
        }
    }
}

void ABlackRemasteredWeapon::PlayMeleeEffects()
{
    // Play melee sound
    // TODO: Add melee sound to weapon data
    
    // Play melee animation
    if (WeaponData.MeleeAnimation && OwnerCharacter)
    {
        if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
        {
            CharacterMesh->PlayAnimation(WeaponData.MeleeAnimation, false);
        }
    }
}

void ABlackRemasteredWeapon::PlayJamEffects()
{
    // Play jam sound
    if (WeaponData.JamSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData.JamSound, GetActorLocation());
    }
}

void ABlackRemasteredWeapon::PlayOverheatEffects()
{
    // Play overheat sound
    if (WeaponData.OverheatSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData.OverheatSound, GetActorLocation());
    }
    
    // Spawn overheat particles
    // TODO: Add overheat particle effect
}

void ABlackRemasteredWeapon::SpawnProjectile()
{
    // This is called by FireProjectile()
    // Implementation is in FireProjectile()
}

FHitResult ABlackRemasteredWeapon::PerformWeaponTrace(FVector& Start, FVector& End)
{
    FHitResult HitResult;
    
    // Set up trace parameters
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(OwnerCharacter);
    TraceParams.AddIgnoredActor(this);
    TraceParams.bTraceComplex = true;
    TraceParams.bReturnPhysicalMaterial = true;
    
    // Perform trace
    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        TraceParams
    );
    
    return HitResult;
}

void ABlackRemasteredWeapon::ApplyRecoil()
{
    // Apply vertical recoil
    float RecoilScale = 1.0f;
    
    if (OwnerCharacter)
    {
        if (OwnerCharacter->IsAiming())
        {
            RecoilScale = 0.7f;
        }
        
        if (OwnerCharacter->IsCrouching() || OwnerCharacter->IsProne())
        {
            RecoilScale *= 0.8f;
        }
        
        if (OwnerCharacter->IsSprinting())
        {
            RecoilScale *= 1.5f;
        }
    }
    
    CurrentVerticalRecoil = FMath::Min(CurrentVerticalRecoil + WeaponData.VerticalRecoil * RecoilScale, WeaponData.VerticalRecoil * 3.0f);
    CurrentHorizontalRecoil = FMath::Min(CurrentHorizontalRecoil + WeaponData.HorizontalRecoil * RecoilScale, WeaponData.HorizontalRecoil * 3.0f);
}

void ABlackRemasteredWeapon::ApplySpread(FVector& Direction)
{
    if (CurrentSpread <= 0.01f)
    {
        return;
    }
    
    // Apply random spread
    float SpreadAngle = FMath::DegreesToRadians(CurrentSpread);
    
    // Generate random direction within spread cone
    float RandomAngle = FMath::FRandRange(0.0f, 2.0f * PI);
    float RandomRadius = FMath::FRandRange(0.0f, SpreadAngle);
    
    FVector SpreadDirection = FVector(
        FMath::Cos(RandomAngle) * RandomRadius,
        FMath::Sin(RandomAngle) * RandomRadius,
        0.0f
    );
    
    // Rotate direction by spread
    FVector RightVector = FVector::CrossProduct(Direction, FVector::UpVector).GetSafeNormal();
    FVector UpVector = FVector::CrossProduct(Direction, RightVector).GetSafeNormal();
    
    Direction = Direction.RotateAngleAxis(SpreadDirection.X, UpVector);
    Direction = Direction.RotateAngleAxis(SpreadDirection.Y, RightVector);
}

void ABlackRemasteredWeapon::ConsumeAmmo()
{
    WeaponData.CurrentAmmo--;
    
    // Broadcast ammo changed event
    OnAmmoChanged.Broadcast();
}

void ABlackRemasteredWeapon::CheckForJam()
{
    if (!WeaponData.bCanJam)
    {
        return;
    }
    
    // Random chance to jam
    if (FMath::FRand() < WeaponData.JamProbability)
    {
        Jam();
    }
}

void ABlackRemasteredWeapon::CheckForOverheat()
{
    if (!WeaponData.bCanOverheat)
    {
        return;
    }
    
    if (CurrentHeat >= WeaponData.OverheatThreshold)
    {
        Overheat();
    }
}
