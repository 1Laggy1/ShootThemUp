// Shoot THem Up Game. All Rights Reserved.

#include "Components/Abilities/STUAbilityOnceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
bool USTUAbilityOnceComponent::StartUseAbility()
{
    bool result = Super::StartUseAbility();
    if (result && UseCount > 0)
    {
        UseCount--;
        AbilityCallback_Multicast();
        return true;
    }
    return false;
}

void USTUAbilityOnceComponent::AbilityCallback()
{
    if (!GetOwner())
        return;
    
    GetWorld()->GetTimerManager().SetTimer(AbilityCooldownTimerHandle, this,
                                           &USTUPlayerAbilityUseComponent::CooldownFinished, Cooldown, false);
    if (UseSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), UseSound, GetOwner()->GetActorLocation());
    }
    if (NiagaraEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, GetOwner()->GetActorLocation(),
                                                       GetOwner()->GetActorRotation());
    }
}

void USTUAbilityOnceComponent::CooldownFinished()
{
    Super::CooldownFinished();
    UseCount++;
}
