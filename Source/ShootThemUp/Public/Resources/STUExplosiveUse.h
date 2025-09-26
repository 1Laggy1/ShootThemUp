// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Resources/STUUseableActor.h"
#include "STUCoreTypes.h"
#include "STUExplosiveUse.generated.h"

/**
 * 
 */
class UNiagaraSystem;
UCLASS()
class SHOOTTHEMUP_API ASTUExplosiveUse : public ASTUUseableActor
{
	GENERATED_BODY()
  public:
    virtual bool Use(FVector Location, FVector Rotation, AController *InstigatedBy) override;

  protected:
    UPROPERTY(EditAnywhere, Category = "Stat")
    FExplosionParams ExplosionStats;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem *NiagaraEffect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    FVector RelativeExplostionPos = FVector::ZeroVector;
};
