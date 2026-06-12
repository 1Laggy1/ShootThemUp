// Shoot THem Up Game. All Rights Reserved.


#include "Components/Abilities/STUSuperJumpAbilityComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
bool USTUSuperJumpAbilityComponent::StartUseAbility()
{
    bool result = Super::StartUseAbility();
    return result;
}

bool USTUSuperJumpAbilityComponent::Ability()
{
    if (MyPlayer && MyPlayer->GetCharacterMovement())
    {
        const bool bOnGround = MyPlayer->GetCharacterMovement()->IsMovingOnGround() ||
                               MyPlayer->GetCharacterMovement()->CurrentFloor.HitResult.bBlockingHit;

        if (bOnGround)
        {
            FVector NewVelocity = MyPlayer->GetVelocity();
            NewVelocity.Z = JumpStrength;
            MyPlayer->LaunchCharacter(NewVelocity, true, true);
            return true;
        }
    }
    return false;
}
