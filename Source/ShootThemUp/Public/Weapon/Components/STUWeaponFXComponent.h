// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponFXComponent.generated.h"


class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponFXComponent();

	void PlayImpactFX(const FHitResult &Hit);

protected:
	virtual void BeginPlay() override;


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
  FImpactData DefaultImpactData;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
  TMap<UPhysicalMaterial *, FImpactData> ImpactDataMap;
		
};
