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
        AbilityCallback_Multicast();
        UseCount--;
        return true;
    }
    return false;
}

void USTUAbilityOnceComponent::AbilityCallback()
{
    if (!GetOwner())
        return;
    if (GetOwner()->HasAuthority() && UseCount == UseCountDefault)
    {
        GetWorld()->GetTimerManager().SetTimer(AbilityCooldownTimerHandle, this,
                                               &USTUPlayerAbilityUseComponent::CooldownFinished, Cooldown, true);
        StartTimer_Client(true);
    }
        
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
    if (!GetOwner()->HasAuthority()) return;
    if (UseCount < UseCountDefault)
    {
        UseCount++;
        if (UseCount == UseCountDefault)
        {
            GetWorld()->GetTimerManager().ClearTimer(AbilityCooldownTimerHandle);
            StartTimer_Client(false);
        }
    }
}
