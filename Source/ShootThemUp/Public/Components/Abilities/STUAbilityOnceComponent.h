// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/STUPlayerAbilityUseComponent.h"
#include "STUAbilityOnceComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTTHEMUP_API USTUAbilityOnceComponent : public USTUPlayerAbilityUseComponent
{
	GENERATED_BODY()
  public:
  protected:
    virtual bool StartUseAbility() override;
    virtual void AbilityCallback() override;
    virtual void CooldownFinished() override;
  private:
    
};
