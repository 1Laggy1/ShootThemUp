// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "Weapon/STUTraceWeapon.h"
#include "STURifleWeapon.generated.h"

/**
 * 
 */
class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUTraceWeapon
{
	GENERATED_BODY()
  public:

    virtual void StartFire() override;
    virtual void StopFire() override;
    
  protected:
    UPROPERTY(EditAnywhere, Category = "Stats")
    float TimeBetweenShots = 0.1f;
    
  private:
    FTimerHandle ShotTimerHandle;
    
};
