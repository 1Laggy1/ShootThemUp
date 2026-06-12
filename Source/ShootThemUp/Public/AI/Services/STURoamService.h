// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STURoamService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTURoamService : public UBTService
{
	GENERATED_BODY()
  public:
    USTURoamService();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool SelfCenter = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector AimLocationKey;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float Radius = 1000.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "!SelfCenter"))
    FBlackboardKeySelector CenterActorKey;
    virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};
