// Shoot THem Up Game. All Rights Reserved.


#include "Weapon/STUTraceWeapon.h"
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
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTUTraceWeapon::ASTUTraceWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTUTraceWeapon::MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                    int32 InstigatorID)
{
    MakeShotMulticast(ViewLocation, ViewRotation, TraceEnd, InstigatorID);
}

void ASTUTraceWeapon::MakeShotFX(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd)
{

    FHitResult HitResult;
    MakeHit(HitResult, ViewLocation, TraceEnd);
    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    ShootMuzzleFX();
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
    if (FireSound)
    {
        UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
}

void ASTUTraceWeapon::MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                       int32 InstigatorID)
{
    if (!Cast<ACharacter>(GetOwner()) || !Cast<ACharacter>(GetOwner())->GetPlayerState() ||
        !Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
        return;

    int32 InstigatorIDLocal = Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID();
    if (InstigatorID == InstigatorIDLocal)
        return;
    MakeShotFX(ViewLocation, ViewRotation, TraceEnd);
}

void ASTUTraceWeapon::MakeShot()
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
    DecreaseAmmo();
    const FVector SocketLocation = GetMuzzleWorldLocation();
    FVector TraceEnd = GetTraceData(ViewLocation, ViewRotator);

    if (GetOwner() && Cast<APawn>(GetOwner())->Controller &&
        Cast<APawn>(GetOwner())->Controller->IsLocalPlayerController())
    {
        FHitResult HitResult;
        MakeHit(HitResult, ViewLocation, TraceEnd);
        if (HitResult.bBlockingHit)
        {
            if (HitResult.GetActor())
            {
                MakeDamage_Server(HitResult);
            }
        }
    }

    MakeShotFX(ViewLocation, ViewRotator, TraceEnd);
    int32 ID = Cast<ACharacter>(GetOwner())->Controller->PlayerState->GetUniqueID();
    if (Cast<ACharacter>(GetOwner())->Controller->IsLocalPlayerController())
        MakeShotServer(ViewLocation, ViewRotator, TraceEnd, ID);
}

void ASTUTraceWeapon::BeginPlay()
{
    Super::BeginPlay();
    SetReplicates(true);
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
        MuzzleFXComponent->SetVisibility(false, true);
    }
}

void ASTUTraceWeapon::Zoom(bool Enable)
{
    const auto Controllerr = Cast<APlayerController>(GetController());
    if (!Controllerr || !Controllerr->PlayerCameraManager)
        return;
    if (Enable)
        BulletSpread = 0.f;
    else
        BulletSpread = BulletSpreadDefault;
    IsZoomed = Enable;
    const TInterval<float> FOV(ZoomFOV, NoZoomFOV);
    Controllerr->PlayerCameraManager->SetFOV(Enable ? FOV.Min : FOV.Max);
}

FVector ASTUTraceWeapon::GetTraceData(FVector ViewLocation, FRotator ViewRotation) const
{
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);

    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    FVector TraceEnd = ViewLocation + ShootDirection * TraceMaxDistance;

    return TraceEnd;
}

void ASTUTraceWeapon::MakeDamage_Server_Implementation(const FHitResult &HitResult)
{
    if (!HitResult.GetActor())
        return;

    FVector TraceStart = GetMuzzleWorldLocation();
    float Distance = FVector::Dist(TraceStart, HitResult.ImpactPoint);

    float ActualDamage = Damage;

    if (Distance > DamageDistanceStartFalloff)
    {
        float Alpha = (Distance - DamageDistanceStartFalloff) / (DamageDistanceEndFalloff - DamageDistanceStartFalloff);

        Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);

        ActualDamage = FMath::Lerp(Damage, MinDamage, Alpha);
    }

    FPointDamageEvent PointDamageEvent;
    PointDamageEvent.HitInfo = HitResult;

    HitResult.GetActor()->TakeDamage(ActualDamage, PointDamageEvent, Cast<ACharacter>(GetOwner())->Controller, this);
}

void ASTUTraceWeapon::ShootMuzzleFX()
{
    if (!MuzzleFXComponent)
        return;

    MuzzleFXComponent->SetPaused(false);
    MuzzleFXComponent->SetVisibility(true, true);
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MuzzleFXTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(MuzzleFXTimerHandle, this, &ASTUTraceWeapon::HideMuzzleFX,
                                               MuzzleFXDuration, false);
    }
}

void ASTUTraceWeapon::HideMuzzleFX()
{
    if (!MuzzleFXComponent)
        return;

    
    MuzzleFXComponent->SetPaused(true);
    MuzzleFXComponent->SetVisibility(false, true);
}

void ASTUTraceWeapon::SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}