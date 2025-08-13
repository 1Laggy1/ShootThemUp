// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
  public:
    virtual void StartFire() override;
    virtual void StopFire() override;

  protected:
    UPROPERTY(EditAnywhere, Category = "Stats")
    float TimeBetweenShots = 0.1f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float BulletSpread = 0.3f;
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd) const override;
    void MakeDamage(AActor *ActorToHit);
  private:
    FTimerHandle ShotTimerHandle;
};
