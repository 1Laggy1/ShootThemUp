// Shoot THem Up Game. All Rights Reserved.


#include "Pickups/STUHealthPickup.h"
#include "STUUtils.h"
#include "Components/STUHealthActorComponent.h"
DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All)
bool ASTUHealthPickup::GivePickupTo(APawn *PlayerPawn)
{
    UE_LOG(LogHealthPickup, Display, TEXT("Health was taken"));
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(PlayerPawn);
    if (!HealthComponent || !HealthComponent->GetIsVaunded())
        return false;
    HealthComponent->TakeHeal(HealAmount);
    return true;
}
