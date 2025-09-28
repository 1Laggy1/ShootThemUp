// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Resources/STUExplosiveUse.h"
#include "STUTVUse.generated.h"

/**
 * 
 */
class AController;
UCLASS()
class SHOOTTHEMUP_API ASTUTVUse : public ASTUExplosiveUse
{
	GENERATED_BODY()
  public:
    virtual bool Use(FVector Location, FVector Rotation, AController *InstigatedBy) override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    float TimeBetweenDamage = 0.5f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    float TimeToDamage = 2.f;
  private:
    FTimerHandle DamageAgainTimerHandle;
    AController *InstigatorController;
    float TimeToDamageRemaining = TimeToDamage;
    void DamageAgain();
};
