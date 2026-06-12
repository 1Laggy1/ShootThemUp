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


void ASTURifleWeapon::StartFire()
{
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
    
}


