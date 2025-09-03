// Shoot THem Up Game. All Rights Reserved.

#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Player/STUPlayerState.h"
#include "Sound/SoundCue.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation, int32 InstigatorID)
{
    MakeShotMulticast(ViewLocation, ViewRotation, InstigatorID);

    const FVector SocketLocation = GetMuzzleWorldLocation();
    FVector TraceEnd = GetTraceData(ViewLocation, ViewRotation);
    FHitResult HitResult;
    MakeHit(HitResult, ViewLocation, TraceEnd);
    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        WeaponFXComponent->PlayImpactFX(HitResult);
        if (HitResult.GetActor())
        {
            MakeDamage(HitResult);
        }
    }
    // DecreaseAmmo();
}

void ASTURifleWeapon::MakeShotFX(FVector ViewLocation, FRotator ViewRotation)
{

    const FVector SocketLocation = GetMuzzleWorldLocation();
    FVector TraceEnd = GetTraceData(ViewLocation, ViewRotation);
    FHitResult HitResult;
    MakeHit(HitResult, ViewLocation, TraceEnd);
    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
    if (FireSound)
    {
        UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
}

void ASTURifleWeapon::MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, int32 InstigatorID)
{
    if (!Cast<ACharacter>(GetOwner()) || !Cast<ACharacter>(GetOwner())->GetPlayerState() ||
        !Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
        return;

    int32 InstigatorIDLocal = Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID();
    if (InstigatorID == InstigatorIDLocal)
        return;
    MakeShotFX(ViewLocation, ViewRotation);
}

void ASTURifleWeapon::MakeShot()
{
    if (!Controller)
    {
        Controller = GetController();
        if (!Controller)
            return;
    }
    if (!GetWorld())
    {
        StopFire();
        return;
    }
    if (isClipEmpty())
    {
        StopFire();
        if (IsAmmoEmpty() && NoAmmoSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
            return;
        }
        return;
    }
    FVector ViewLocation = FVector();
    FRotator ViewRotator = FRotator();
    GetPlayerViewPoint(ViewLocation, ViewRotator);
    /*if (!GetOwner()->HasAuthority())
    {
       */
    DecreaseAmmo();
    //}

    MakeShotFX(ViewLocation, ViewRotator);

    MakeShotServer(ViewLocation, ViewRotator, Controller->PlayerState->GetUniqueID());
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    SetReplicates(true);
}

void ASTURifleWeapon::StartFire()
{
    InitMuzzleFX();
    if (!GetWorldTimerManager().IsTimerActive(ShotTimerHandle))
    {
        if (isClipEmpty())
        {
            StopFire();
            if (NoAmmoSound)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
            }

            return;
        }
        MakeShot();
        GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    }
}
void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::Zoom(bool Enable)
{
    const auto Controllerr = Cast<APlayerController>(GetController());
    if (!Controllerr || !Controllerr->PlayerCameraManager)
        return;

    const TInterval<float> FOV(ZoomFOV, NoZoomFOV);
    Controllerr->PlayerCameraManager->SetFOV(Enable ? FOV.Min : FOV.Max);
}

FVector ASTURifleWeapon::GetTraceData(FVector ViewLocation, FRotator ViewRotation) const
{
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    FVector TraceEnd = ViewLocation + ShootDirection * TraceMaxDistance;

    return TraceEnd;
}
void ASTURifleWeapon::MakeDamage(const FHitResult &HitResult)
{
    FPointDamageEvent PointDamageEvent;
    PointDamageEvent.HitInfo = HitResult;
    HitResult.GetActor()->TakeDamage(Damage, PointDamageEvent, Cast<ACharacter>(GetOwner())->Controller, this);
}

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
    if (!MuzzleFXComponent)
        return;

    MuzzleFXComponent->SetPaused(!Visible);
    MuzzleFXComponent->SetVisibility(Visible, true);
}

void ASTURifleWeapon::SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}
