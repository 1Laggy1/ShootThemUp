// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

/**
 * 
 */
class ASTUProjectile;

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
  public:
    virtual void StartFire() override;

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUProjectile> ProjectileClass;
    virtual void MakeShot() override;
    int ShotsFires = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int ShotsToFire = 3;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    float ShotInterval = 0.2f;
    FTimerHandle BurstTimerHandle;
    void MakeBurstShot();
};
