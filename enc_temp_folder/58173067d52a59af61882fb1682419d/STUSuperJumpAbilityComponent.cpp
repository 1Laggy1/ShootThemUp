// Shoot THem Up Game. All Rights Reserved.


#include "Components/Abilities/STUSuperJumpAbilityComponent.h"
#include "GameFramework/Character.h"

bool USTUSuperJumpAbilityComponent::StartUseAbility()
{
    bool result = Super::StartUseAbility();
    if (!result)
        return false;
    SuperJump();
    return true;
}

void USTUSuperJumpAbilityComponent::SuperJump()
{
    FVector NewVelocity = MyPlayer->GetVelocity();
    NewVelocity.Z = JumpStrength;
    MyPlayer->LaunchCharacter(NewVelocity, /*bXYOverride=*/true, /*bZOverride=*/true);
}
