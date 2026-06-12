// Shoot THem Up Game. All Rights Reserved.


#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/STUBasePickup.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance &QueryInstance) const
{
    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor *ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
        if (!PickupActor)
            continue;

        const auto CouldBeTaken = PickupActor->CouldBeTaken();
        if (CouldBeTaken)
        {
            It.SetScore(TestPurpose, FilterType, true, true);
        }
        else
        {
            It.ForceItemState(EEnvItemStatus::Failed);
        }
    }

}
