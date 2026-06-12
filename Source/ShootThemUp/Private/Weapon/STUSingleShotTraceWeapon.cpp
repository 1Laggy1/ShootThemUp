// Shoot THem Up Game. All Rights Reserved.

#include "Weapon/STUSingleShotTraceWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASTUSingleShotTraceWeapon::StartFire()
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
}