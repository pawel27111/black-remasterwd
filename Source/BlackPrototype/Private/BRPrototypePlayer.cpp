#include "BRPrototypePlayer.h"
#include "BRPrototypeGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RotationMatrix.h"
#include "TimerManager.h"

ABRPrototypePlayer::ABRPrototypePlayer()
{
    SetCanBeDamaged(true);
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(GetCapsuleComponent());
    ViewCamera->SetRelativeLocation(FVector(0, 0, 65));
    ViewCamera->bUsePawnControlRotation = true;
    bUseControllerRotationYaw = true;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = 600;
    GetCharacterMovement()->JumpZVelocity = 620;
}

void ABRPrototypePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABRPrototypePlayer::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABRPrototypePlayer::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABRPrototypePlayer::Fire);
    PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABRPrototypePlayer::Reload);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ABRPrototypePlayer::StartSprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ABRPrototypePlayer::StopSprint);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Restart"), IE_Pressed, this, &ABRPrototypePlayer::Restart);
    FInputActionBinding& PauseBinding = PlayerInputComponent->BindAction(
        TEXT("Pause"), IE_Pressed, this, &ABRPrototypePlayer::TogglePause);
    PauseBinding.bExecuteWhenPaused = true;
}

void ABRPrototypePlayer::MoveForward(float Value)
{
    if (Controller && Value != 0)
    {
        AddMovementInput(FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X), Value);
    }
}

void ABRPrototypePlayer::MoveRight(float Value)
{
    if (Controller && Value != 0)
    {
        AddMovementInput(FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y), Value);
    }
}

void ABRPrototypePlayer::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 950;
}

void ABRPrototypePlayer::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 600;
}

void ABRPrototypePlayer::Fire()
{
    if (bReloading || AmmoInMagazine <= 0 || UGameplayStatics::IsGamePaused(this))
    {
        return;
    }

    const float Now = GetWorld()->GetTimeSeconds();
    if (Now - LastShotTime < 0.14f)
    {
        return;
    }
    LastShotTime = Now;
    --AmmoInMagazine;

    const FVector Start = ViewCamera->GetComponentLocation();
    const FVector Direction = ViewCamera->GetForwardVector();
    const FVector End = Start + Direction * 12000.0f;
    FCollisionQueryParams Query(SCENE_QUERY_STAT(PrototypeShot), true, this);
    FHitResult Hit;
    const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Query);
    DrawDebugLine(GetWorld(), Start, bHit ? Hit.ImpactPoint : End,
        FColor::Yellow, false, 0.08f, 0, 1.5f);

    if (bHit && Hit.GetActor())
    {
        UGameplayStatics::ApplyPointDamage(Hit.GetActor(), 34.0f, Direction,
            Hit, GetController(), this, UDamageType::StaticClass());
    }
}

void ABRPrototypePlayer::Reload()
{
    if (bReloading || AmmoInMagazine == 30 || ReserveAmmo <= 0)
    {
        return;
    }
    bReloading = true;
    GetWorldTimerManager().SetTimer(ReloadTimer, this, &ABRPrototypePlayer::FinishReload, 1.3f, false);
}

void ABRPrototypePlayer::FinishReload()
{
    const int32 Amount = FMath::Min(30 - AmmoInMagazine, ReserveAmmo);
    AmmoInMagazine += Amount;
    ReserveAmmo -= Amount;
    bReloading = false;
}

float ABRPrototypePlayer::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Health = FMath::Max(0.0f, Health - Applied);
    if (Health <= 0.0f)
    {
        Restart();
    }
    return Applied;
}

void ABRPrototypePlayer::ResetForRound()
{
    GetWorldTimerManager().ClearTimer(ReloadTimer);
    bReloading = false;
    Health = 100.0f;
    AmmoInMagazine = 30;
    ReserveAmmo = 90;
    LastShotTime = -1.0f;
    SetActorLocation(FVector(-1100, 0, 160), false, nullptr, ETeleportType::TeleportPhysics);
    if (Controller)
    {
        Controller->SetControlRotation(FRotator::ZeroRotator);
    }
    StopSprint();
}

void ABRPrototypePlayer::Restart()
{
    if (ABRPrototypeGameMode* Mode = GetWorld()->GetAuthGameMode<ABRPrototypeGameMode>())
    {
        Mode->RestartArena();
    }
}

void ABRPrototypePlayer::TogglePause()
{
    UGameplayStatics::SetGamePaused(this, !UGameplayStatics::IsGamePaused(this));
}
