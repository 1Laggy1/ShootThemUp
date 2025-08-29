// Shoot THem Up Game. All Rights Reserved.


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult &Hit)
{
    auto ImpactData = DefaultImpactData;

	if (Hit.PhysMaterial.IsValid())
	{
        const auto PhysMat = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMat))
		{
            ImpactData = ImpactDataMap[PhysMat];
		}
	}
	// niagara
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, Hit.ImpactPoint,
                                                   Hit.ImpactNormal.Rotation());
	// decal
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),                    //
                                           ImpactData.DecalData.Material, //
                                           ImpactData.DecalData.Size,     //
                                           Hit.ImpactPoint,               //
                                           Hit.ImpactNormal.Rotation());
	if (ImpactData.ImpactSound)
	{
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.ImpactSound, FVector(Hit.ImpactPoint));
	}
	if (DecalComponent)
	{
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
	}
}


void USTUWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

