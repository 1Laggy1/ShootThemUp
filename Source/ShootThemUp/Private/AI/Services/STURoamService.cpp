// Shoot THem Up Game. All Rights Reserved.


#include "AI/Services/STURoamService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/STUAIController.h"
#include "NavigationSystem.h"

USTURoamService::USTURoamService()
{
    NodeName = "Roam";
}

void USTURoamService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard)
        return;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
        return;

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys)
        return;

    FNavLocation NavLocation;
    auto Location = Pawn->GetActorLocation();
    if (!SelfCenter)
    {
        auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor)
            return;
        Location = CenterActor->GetActorLocation();
    }

    const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
    if (!Found)
        return;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return;
}
