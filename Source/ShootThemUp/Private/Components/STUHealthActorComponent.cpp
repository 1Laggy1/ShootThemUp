// Shoot THem Up Game. All Rights Reserved.

#include "Components/STUHealthActorComponent.h"

USTUHealthActorComponent::USTUHealthActorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthActorComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
}
