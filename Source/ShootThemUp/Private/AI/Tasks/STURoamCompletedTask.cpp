// Shoot THem Up Game. All Rights Reserved.


#include "AI/Tasks/STURoamCompletedTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/STUAIController.h"

USTURoamCompletedTask::USTURoamCompletedTask()
{
    NodeName = "EraseLocations";
}

EBTNodeResult::Type USTURoamCompletedTask::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        Blackboard->ClearValue(ToErase.SelectedKeyName);
    }
    else
    {
        return EBTNodeResult::Failed;
    }
    return EBTNodeResult::Succeeded;
}
