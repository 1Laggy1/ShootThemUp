// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUJumppad.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASTUJumppad::Interact(AActor *OtherActor)
{
    Super::Interact(OtherActor);
    ACharacter *character = Cast<ACharacter>(OtherActor);
    
    if (character)
    { 
        Jump(character);
    }
}

void ASTUJumppad::Jump(ACharacter *character)
{
    if (!character)
    {
        return;
    }

    float Gravity = FMath::Abs(character->GetCharacterMovement()->GetGravityZ());

    float HeightInCm = JumpHeight * 100.0f;

    float RequiredVelocityZ = FMath::Sqrt(2.0f * Gravity * HeightInCm);

    FVector NewVelocity = character->GetVelocity();
    NewVelocity.Z = RequiredVelocityZ;

    character->LaunchCharacter(NewVelocity, true, true);
}
