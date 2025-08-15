// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
DEFINE_LOG_CATEGORY_STATIC(LogHudWidget, All, All)
float USTUPlayerHUDWidget::GetHealthPercent() const
{
    auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(GetOwningPlayerPawn());
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

FString USTUPlayerHUDWidget::GetCurrentAmmo() const
{
    FAmmoData AmmoData;
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent || !WeaponComponent->GetCurrentAmmoData(AmmoData))
    {
        return "0/0";
    }
    FString AmmoInfo = FString::FromInt(AmmoData.Bullets) + " / ";
    AmmoInfo += AmmoData.Infinite ? FString::Chr(0x221E) : FString::FromInt(AmmoData.Clips);
    //UE_LOG(LogHudWidget, Warning, *Ammo);
    return AmmoInfo;
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent)
        return false;

    WeaponComponent->GetWeaponUIData(UIData);
    return true;

}

bool USTUPlayerHUDWidget::isPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->isDead();
}

bool USTUPlayerHUDWidget::isPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}


