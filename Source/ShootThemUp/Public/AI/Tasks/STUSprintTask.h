// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STUSprintTask.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUSprintTask : public UBTTaskNode
{
	GENERATED_BODY()
  public:
    USTUSprintTask();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool Sprint;
};
