// Shoot THem Up Game. All Rights Reserved.


#include "Components/Abilities/STUDashAbilityComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

bool USTUDashAbilityComponent::StartUseAbility()
{
    bool result = Super::StartUseAbility();
    return result;
}

bool USTUDashAbilityComponent::Ability()
{
    APlayerController *PC = Cast<APlayerController>(MyPlayer->GetController());
    if (!PC)
        return false;

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector CameraForward = CameraRotation.Vector();
    CameraForward.Z = 0.f;
    CameraForward.Normalize();

    const float ForwardInput = MyPlayer->GetInputAxisValue(TEXT("MoveForward"));
    const float RightInput = MyPlayer->GetInputAxisValue(TEXT("MoveRight"));

    FVector DashDirection = FVector::ZeroVector;

    if (FMath::Abs(ForwardInput) < KINDA_SMALL_NUMBER && FMath::Abs(RightInput) < KINDA_SMALL_NUMBER)
    {
        DashDirection = CameraForward;
    }
    else
    {
        FVector CameraRight = CameraRotation.RotateVector(FVector::RightVector);
        CameraRight.Z = 0.f;
        CameraRight.Normalize();

        DashDirection = CameraForward * ForwardInput + CameraRight * RightInput;
        DashDirection.Z = 0.f;

        if (DashDirection.IsNearlyZero())
            DashDirection = CameraForward;
        else
            DashDirection.Normalize();
    }

    FVector CameraForwardFull = CameraRotation.Vector();
    float PitchZ = FMath::Clamp(CameraForwardFull.Z, 0.0f, 1.0f);
    float PitchBoost = PitchZ * PitchUpMultiplier * DashStrength;
    float PitchFactor = FMath::Clamp(PitchZ * PitchHorizontalReduction, 0.0f, 1.0f);
    FVector DashDirXY = DashDirection;
    DashDirXY.Z = 0.f;
    if (DashDirXY.IsNearlyZero())
    {
        DashDirXY = CameraRotation.Vector();
        DashDirXY.Z = 0.f;
    }
    DashDirXY.Normalize();
    FVector DashVelocityXY = DashDirXY * DashStrength;
    FVector ScaledDashXY = DashVelocityXY * (1.0f - PitchFactor);
    if (ScaledDashXY.SizeSquared() < KINDA_SMALL_NUMBER)
    {
        ScaledDashXY = FVector::ZeroVector;
    }
    FVector NewVelocity = MyPlayer->GetCharacterMovement()->Velocity;
    NewVelocity.X = ScaledDashXY.X;
    NewVelocity.Y = ScaledDashXY.Y;
    NewVelocity.Z = DashUpStrength + PitchBoost;

    MyPlayer->LaunchCharacter(NewVelocity, /*bXYOverride=*/true, /*bZOverride=*/true);
    return true;
}
