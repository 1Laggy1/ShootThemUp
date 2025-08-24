// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STURoamCompletedTask.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTURoamCompletedTask : public UBTTaskNode
{
	GENERATED_BODY()
  public:
    USTURoamCompletedTask();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector ToErase;
};
