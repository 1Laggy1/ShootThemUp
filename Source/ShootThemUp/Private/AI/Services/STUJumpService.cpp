// Shoot THem Up Game. All Rights Reserved.


#include "AI/Services/STUJumpService.h"
#include "GameFramework/Character.h"
#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

USTUJumpService::USTUJumpService()
{
    NodeName = "Jumping";
}

void USTUJumpService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    const auto Controller = Cast<ASTUAIController>(OwnerComp.GetAIOwner());
    const auto Character = Cast<ASTUAICharacter>(Controller->GetPawn());
    float Now = OwnerComp.GetWorld()->GetTimeSeconds();
    
    TimeNow += DeltaSeconds;
    if (Character->GetCharacterMovement() && Character->GetCharacterMovement()->IsMovingOnGround() && TimeNow > JumpCooldown)
    {
        TimeNow = 0;
        Character->Jump();
    }
}
