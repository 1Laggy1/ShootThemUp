// Shoot THem Up Game. All Rights Reserved.


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"

void ASTURifleWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}
void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{

    if (!GetWorld())
        return;

    if (Controller == nullptr)
    {
        Controller = GetPlayerController();
        if (Controller == nullptr)
            return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    const FVector SocketLocation = GetMuzzleWorldLocation();

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
    if (HitResult.bBlockingHit)
    {

        DrawDebugLine(GetWorld(), SocketLocation, HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        if (HitResult.GetActor())
        {
            MakeDamage(HitResult.GetActor());
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), SocketLocation, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
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
void ASTURifleWeapon::MakeDamage(AActor *ActorToHit)
{
    ActorToHit->TakeDamage(Damage, FDamageEvent(), Controller, this);
}