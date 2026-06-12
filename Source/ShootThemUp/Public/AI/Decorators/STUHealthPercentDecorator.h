// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUHealthPercentDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUHealthPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()
  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
    float HealthPercent = 0.6f;
    USTUHealthPercentDecorator();
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const override;
};
