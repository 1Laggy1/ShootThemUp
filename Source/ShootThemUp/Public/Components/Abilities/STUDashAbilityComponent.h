// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/STUAbilityOnceComponent.h"
#include "STUDashAbilityComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUDashAbilityComponent : public USTUAbilityOnceComponent
{
	GENERATED_BODY()
  protected:
    virtual bool StartUseAbility() override;
    void Dash();
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    float DashStrength = 1000.f;

};
