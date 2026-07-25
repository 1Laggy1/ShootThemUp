// Shoot THem Up Game. All Rights Reserved.


#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"
USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
        return false;

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent)
        return false;

    FAmmoData Ammo;
    if (!WeaponComponent->GetCurrentAmmoData(Ammo))
        return false;
    return Ammo.Clips == 0;
}
