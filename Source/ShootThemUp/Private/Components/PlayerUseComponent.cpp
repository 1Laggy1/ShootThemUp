// Shoot THem Up Game. All Rights Reserved.


#include "Components/PlayerUseComponent.h"
#include "Resources/STUUseableActor.h"

UPlayerUseComponent::UPlayerUseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayerUseComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPlayerUseComponent::UseAnItem_Implementation()
{
    if (Item)
    {
		Item->Use();
        Item = nullptr;
    }
}


void UPlayerUseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

