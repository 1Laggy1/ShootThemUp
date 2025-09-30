// Shoot THem Up Game. All Rights Reserved.


#include "Components/Abilities/STUDashAbilityComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

bool USTUDashAbilityComponent::StartUseAbility()
{
    bool result = Super::StartUseAbility();
    if (!result)
        return false;
    Dash();
    return true;
}

void USTUDashAbilityComponent::Dash()
{
    if (!MyPlayer)
        return;

    APlayerController *PC = Cast<APlayerController>(MyPlayer->GetController());
    if (!PC)
        return;

    FVector CamLoc;
    FRotator CamRot;
    PC->GetPlayerViewPoint(CamLoc, CamRot);

    FVector Dir = CamRot.Vector();
    Dir.Z = 0.f;
    if (Dir.IsNearlyZero())
        return;
    Dir.Normalize();

    UCharacterMovementComponent *MoveComp = MyPlayer->GetCharacterMovement();
    if (!MoveComp)
        return;

    FVector Impulse = Dir * DashStrength;
    MoveComp->AddImpulse(Impulse, /*bVelocityChange=*/true);

    MoveComp->GroundFriction = 0.f;
    MoveComp->BrakingDecelerationWalking = 0.f;
}
