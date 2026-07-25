// Shoot THem Up Game. All Rights Reserved.


#include "AI/EQS/STUEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance &QueryInstance, FEnvQueryContextData &ContextData) const
{
    AActor *QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard)
        return;

    const auto ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName);
    UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextActor));
}
