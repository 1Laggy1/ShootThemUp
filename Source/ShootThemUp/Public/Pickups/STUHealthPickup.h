// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
	GENERATED_BODY()
  public:
    UPROPERTY(EditAnywhere, Category = "Heal", meta = (ClampMin = "0"))
    int32 HealAmount = 50;
  private:
    virtual bool GivePickupTo(APawn *PlayerPawn) override;
};
