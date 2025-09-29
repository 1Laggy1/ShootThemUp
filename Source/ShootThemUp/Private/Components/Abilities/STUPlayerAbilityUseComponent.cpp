// Shoot THem Up Game. All Rights Reserved.

#include "Components/Abilities/STUPlayerAbilityUseComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"

USTUPlayerAbilityUseComponent::USTUPlayerAbilityUseComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USTUPlayerAbilityUseComponent::StartUseAbility_Server_Implementation()
{
    StartUseAbility();
}

void USTUPlayerAbilityUseComponent::StopUseAbility_Server_Implementation()
{

    StopUseAbility();
}

void USTUPlayerAbilityUseComponent::AbilityCallback_Multicast_Implementation()
{
    AbilityCallback();
}
void USTUPlayerAbilityUseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USTUPlayerAbilityUseComponent, UseCount);
}
bool USTUPlayerAbilityUseComponent::StartUseAbility()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(AbilityCooldownTimerHandle))
        return false;
    return true;
}

bool USTUPlayerAbilityUseComponent::StopUseAbility()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(AbilityCooldownTimerHandle))
        return false;
    return true;
}

void USTUPlayerAbilityUseComponent::AbilityCallback()
{

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), UseSound, GetOwner()->GetActorLocation());

    if (NiagaraEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, GetOwner()->GetActorLocation(),
                                                       GetOwner()->GetActorRotation());
    }
}

void USTUPlayerAbilityUseComponent::BeginPlay()
{
    Super::BeginPlay();
    if (GetOwner())
    {
        MyPlayer = Cast<ACharacter>(GetOwner());
    }
}

void USTUPlayerAbilityUseComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
