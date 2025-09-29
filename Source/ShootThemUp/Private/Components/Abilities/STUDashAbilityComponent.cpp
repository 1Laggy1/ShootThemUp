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
    APlayerController *PC = Cast<APlayerController>(MyPlayer->GetController());
    if (!PC)
        return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector DashDirection = CameraLocation;
    DashDirection.Normalize();

    FVector DashVelocity = DashDirection * DashStrength;
    FVector NewVelocity = MyPlayer->GetCharacterMovement()->Velocity;
    NewVelocity.X += DashVelocity.X;
    NewVelocity.Y += DashVelocity.Y;
    NewVelocity.Z = DashVelocity.Z;

    MyPlayer->GetCharacterMovement()->Velocity = NewVelocity;


}
