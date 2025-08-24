// Shoot THem Up Game. All Rights Reserved.


#include "AI/Tasks/STUSprintTask.h"
#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"

USTUSprintTask::USTUSprintTask()
{
    NodeName = "Sprint";
}

EBTNodeResult::Type USTUSprintTask::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    const auto Controller = Cast<ASTUAIController>(OwnerComp.GetAIOwner());
    const auto Character = Cast<ASTUAICharacter>(Controller->GetPawn());
    if (Character->isSprintingPressed == Sprint)
        return EBTNodeResult::Succeeded;

    if (Sprint)
    {
        Character->StartSprint();
        //Character->isWalking = true;
    }
    else
    {
        Character->StopSprint();
        //Character->isWalking = false;
    }
    return EBTNodeResult::Succeeded;
}
