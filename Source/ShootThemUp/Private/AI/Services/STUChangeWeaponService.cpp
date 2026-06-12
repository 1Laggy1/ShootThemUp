// Shoot THem Up Game. All Rights Reserved.


#include "AI/Services/STUChangeWeaponService.h"
#include "GameFramework/Character.h"
#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "Components/STUAIWeaponComponent.h"
#include "STUUtils.h"


USTUChangeWeaponService::USTUChangeWeaponService()
{

}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    TimeNow += DeltaSeconds;
    if (TimeNow > ChangeCooldown)
    {
        TimeNow = 0;
        float Rand = FMath::RandRange(0.0, 1.0);
        if (Rand < Probability)
        {
            const auto Controller = OwnerComp.GetAIOwner();
            const auto Character = Cast<ASTUAICharacter>(Controller->GetPawn());
            const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUAIWeaponComponent>(Character);
            WeaponComponent->NextWeapon();
        }
        
    }
    

}
