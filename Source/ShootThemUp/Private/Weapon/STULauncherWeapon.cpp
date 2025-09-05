// Shoot THem Up Game. All Rights Reserved.


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Player/STUPlayerState.h"

void ASTULauncherWeapon::StartFire()
{
    if (GetWorldTimerManager().IsTimerActive(BurstTimerHandle))
        return;

    GetWorldTimerManager().SetTimer(BurstTimerHandle, this, &ASTULauncherWeapon::MakeBurstShot, ShotInterval, true);
}



void ASTULauncherWeapon::MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                          int32 InstigatorID)
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


    const FVector SocketLocation = GetMuzzleWorldLocation();

    FHitResult HitResult;
    MakeHit(HitResult, ViewLocation, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - SocketLocation).GetSafeNormal();
    
    SpawnProjectileServer(Direction);
    
    // set projectile params
    if (FireSound)
    {
        UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
    DecreaseAmmo();
    SpawnMuzzleFX();
}

void ASTULauncherWeapon::SpawnProjectileServer_Implementation(FVector Direction)
{

     const FVector SocketLocation = GetMuzzleWorldLocation();

    const FTransform SpawnTransform(FRotator::ZeroRotator, SocketLocation);

     

    ASTUProjectile *Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
}

void ASTULauncherWeapon::MakeBurstShot()
{
    if (ShotsFires >= ShotsToFire)
    {
        ShotsFires = 0;
        GetWorldTimerManager().ClearTimer(BurstTimerHandle);
        return;
    }
        FVector ViewLocation;
        FRotator ViewRotation;
        GetPlayerViewPoint(ViewLocation, ViewRotation);
        ShotsFires++;

        FVector TraceEnd = GetTraceData(ViewLocation, ViewRotation);
        MakeShotServer(ViewLocation, ViewRotation, TraceEnd, Controller->PlayerState->GetPlayerId());

}
