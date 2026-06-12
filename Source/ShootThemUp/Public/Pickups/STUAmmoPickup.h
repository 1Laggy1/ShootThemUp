// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

/**
 * 
 */
class ASTUBaseWeapon;
UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
	GENERATED_BODY()
  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
    int32 ClipsAmount = 5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    TSubclassOf<ASTUBaseWeapon> WeaponType;
  private:
    virtual bool GivePickupTo(APawn *PlayerPawn) override;
};
