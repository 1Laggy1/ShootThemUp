// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUJumpService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUJumpService : public UBTService
{
	GENERATED_BODY()
  public:
    float TimeNow;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float JumpCooldown;
    USTUJumpService();
    virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};
