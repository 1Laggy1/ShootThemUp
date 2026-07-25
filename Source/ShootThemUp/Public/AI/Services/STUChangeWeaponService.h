// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUChangeWeaponService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUChangeWeaponService : public UBTService
{
	GENERATED_BODY()
  public:
    float TimeNow;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChangeCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
    float Probability = 1;
    USTUChangeWeaponService();
    virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};
