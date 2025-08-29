// Shoot THem Up Game. All Rights Reserved.


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASTULauncherWeapon::StartFire()
{
    if (GetWorldTimerManager().IsTimerActive(BurstTimerHandle))
        return;

    GetWorldTimerManager().SetTimer(BurstTimerHandle, this, &ASTULauncherWeapon::MakeBurstShot, ShotInterval, true);
}



void ASTULauncherWeapon::MakeShot()
{
    

    if (!GetWorld())
        return;
    if (IsAmmoEmpty())
    {
        if (NoAmmoSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        }
        return;
    }
    if (Controller == nullptr)
    {
        Controller = GetController();
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
    if (FireSound)
    {
        UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
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
