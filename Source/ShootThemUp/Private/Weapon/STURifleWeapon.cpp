// Shoot THem Up Game. All Rights Reserved.


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    
}

void ASTURifleWeapon::StartFire()
{
    InitMuzzleFX();
    MakeShot();
    if (!GetWorldTimerManager().IsTimerActive(ShotTimerHandle))
    {
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



void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld())
    {
        StopFire();
        return;
    }
    if (IsAmmoEmpty())
    {
        StopFire();
        if (NoAmmoSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        }
        
        return;
    }
    Controller = GetController();
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }
    const FVector SocketLocation = GetMuzzleWorldLocation();

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        //*DrawDebugLine(GetWorld(), SocketLocation, HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);*/
        WeaponFXComponent->PlayImpactFX(HitResult);
        if (HitResult.GetActor())
        {
            MakeDamage(HitResult);
        }
    }
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
    if (FireSound)
    {
        UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
   
    DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}
void ASTURifleWeapon::MakeDamage(const FHitResult &HitResult)
{
    FPointDamageEvent PointDamageEvent;
    PointDamageEvent.HitInfo = HitResult;
    HitResult.GetActor()->TakeDamage(Damage, PointDamageEvent, Controller, this);
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
