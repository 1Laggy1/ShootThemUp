// Shoot THem Up Game. All Rights Reserved.


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

void ASTULauncherWeapon::StartFire()
{
    if (GetWorldTimerManager().IsTimerActive(BurstTimerHandle))
        return;

    GetWorldTimerManager().SetTimer(BurstTimerHandle, this, &ASTULauncherWeapon::MakeBurstShot, ShotInterval, true);
}



void ASTULauncherWeapon::MakeShot()
{

    if (!GetWorld() || IsAmmoEmpty())
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

    const FTransform SpawnTransform(FRotator::ZeroRotator, SocketLocation);
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - SocketLocation).GetSafeNormal();

    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    // set projectile params

    DecreaseAmmo();
    SpawnMuzzleFX();
}

void ASTULauncherWeapon::MakeBurstShot()
{
    if (ShotsFires >= ShotsToFire)
    {
        ShotsFires = 0;
        GetWorldTimerManager().ClearTimer(BurstTimerHandle);
        return;
    }
    
        ShotsFires++;
        MakeShot();

}
