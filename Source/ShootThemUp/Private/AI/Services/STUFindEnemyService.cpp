// Shoot THem Up Game. All Rights Reserved.

#include "AI/Services/STUFindEnemyService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUAIPerceptionComponent.h"
#include "Components/STUHealthActorComponent.h"
#include "GameFramework/Character.h"
#include "STUUtils.h"
#include "AI/STUAIController.h"

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "Find Enemy";
    bNotifyOnSearch = true;
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
        if (PerceptionComponent)
        {
            AActor *ClosestEnemy = PerceptionComponent->GetClosestEnemy();
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, ClosestEnemy);
            if (ClosestEnemy)
            {
                Blackboard->SetValueAsVector(LastEnemyPositionActorKey.SelectedKeyName,
                                             ClosestEnemy->GetActorLocation());
            }
            /*AActor *DamageEnemy = PerceptionComponent->GetDamageEnemy();
            if (DamageEnemy)
            {
                Blackboard->SetValueAsVector(DamageEnemyPositionKey.SelectedKeyName, DamageEnemy->GetActorLocation());
            }*/
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
