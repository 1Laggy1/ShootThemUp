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
    virtual bool Ability() override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
    float DashStrength = 1000.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
    float DashUpStrength = 200.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
    float PitchUpMultiplier = 0.5f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
    float PitchHorizontalReduction = 0.5f;
};
