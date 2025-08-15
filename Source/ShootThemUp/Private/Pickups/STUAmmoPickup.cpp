// Shoot THem Up Game. All Rights Reserved.


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All)
bool ASTUAmmoPickup::GivePickupTo(APawn *PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->isDead())
        return false;
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
