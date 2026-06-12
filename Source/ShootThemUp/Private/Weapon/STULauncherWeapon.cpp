// Shoot THem Up Game. All Rights Reserved.

#include "Weapon/STULauncherWeapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STUPlayerState.h"
#include "Sound/SoundCue.h"
#include "Weapon/STUProjectile.h"

void ASTULauncherWeapon::StartFire()
{
    if (GetWorldTimerManager().IsTimerActive(BurstTimerHandle))
        return;

    GetWorldTimerManager().SetTimer(BurstTimerHandle, this, &ASTULauncherWeapon::MakeBurstShot, ShotInterval, true);
}

void ASTULauncherWeapon::MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
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
    MakeShotMulticast(ViewLocation, ViewRotation, TraceEnd, InstigatorID);
    //DecreaseAmmo();
}

void ASTULauncherWeapon::MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                          int32 InstigatorID)
{
    if (!Cast<ACharacter>(GetOwner()) || !Cast<ACharacter>(GetOwner())->GetPlayerState() ||
        !Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
        return;
    if (Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID() == InstigatorID)
        return;
    /*UE_LOG(LogTemp, Warning, TEXT("Launcher MakeShotMulticast : %s : %s"),
           *FString::FromInt(Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID()),
           *FString::FromInt(InstigatorID));*/
    FakeShot(ViewLocation, ViewRotation, TraceEnd);
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
    if (!GetOwner() || !Cast<ACharacter>(GetOwner())->Controller)
        return;
    if (IsAmmoEmpty())
    {
        ShotsFires = 0;
        GetWorldTimerManager().ClearTimer(BurstTimerHandle);
        if (NoAmmoSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        }
        return;
    }
    if (ShotsFires >= ShotsToFire)
    {
        ShotsFires = 0;
        GetWorldTimerManager().ClearTimer(BurstTimerHandle);
        return;
    }
    
    DecreaseAmmo();
    ShotsFires++;
    FVector ViewLocation = FVector();
    FRotator ViewRotator = FRotator();
    GetPlayerViewPoint(ViewLocation, ViewRotator);
    const FVector SocketLocation = GetMuzzleWorldLocation();
    FVector TraceEnd = GetTraceData(ViewLocation, ViewRotator);
    FakeShot(ViewLocation, ViewRotator, TraceEnd);
    int32 ID = Cast<ACharacter>(GetOwner())->Controller->PlayerState->GetUniqueID();
    MakeShotServer(ViewLocation, ViewRotator, TraceEnd, ID);
}

void ASTULauncherWeapon::FakeShot(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd)
{
    SpawnMuzzleFX();
    UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    const FVector SocketLocation = GetMuzzleWorldLocation();

    const FTransform SpawnTransform(FRotator::ZeroRotator, SocketLocation);

    ASTUProjectile *Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    
    if (Projectile)
    {
        Projectile->SetReplicates(false);

        const FVector Direction = (TraceEnd - SocketLocation).GetSafeNormal();
        /*UE_LOG(LogTemp, Warning, TEXT("SOCKET LOCATION: %s TRACE END: %s DIRECTION: %s "),
               *SocketLocation.ToString(),
               *TraceEnd.ToString(), *Direction.ToString());*/
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        if (Projectile->GetCollisionComponent())
        {
            Projectile->GetCollisionComponent()->IgnoreActorWhenMoving(GetOwner(), true);
            Projectile->GetCollisionComponent()->IgnoreActorWhenMoving(GetOwner()->GetOwner(), true);
            Projectile->GetCollisionComponent()->IgnoreActorWhenMoving(this, true);
        }
        Projectile->FinishSpawning(SpawnTransform);
    }
}