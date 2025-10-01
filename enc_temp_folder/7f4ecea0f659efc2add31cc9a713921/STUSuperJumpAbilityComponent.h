// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/STUAbilityOnceComponent.h"
#include "STUSuperJumpAbilityComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUSuperJumpAbilityComponent : public USTUAbilityOnceComponent
{
	GENERATED_BODY()
  public:
    virtual bool StartUseAbility() override;
    void SuperJump();
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SuperJump")
    float JumpStrength = 2000.f;
};
