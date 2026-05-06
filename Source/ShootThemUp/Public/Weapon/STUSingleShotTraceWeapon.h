// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUTraceWeapon.h"
#include "STUSingleShotTraceWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUSingleShotTraceWeapon : public ASTUTraceWeapon
{
	GENERATED_BODY()
  public:
    virtual void StartFire() override;
};
