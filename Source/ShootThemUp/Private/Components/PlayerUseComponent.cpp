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
void UPlayerUseComponent::Use()
{
    FVector CameraLocation;
    FRotator CameraRotation;
    Cast<APawn>(GetOwner())->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
    UseAnItem(GetOwner()->GetActorLocation(), CameraRotation.Vector());
}
void UPlayerUseComponent::UseAnItem_Implementation(FVector Location, FVector Rotation)
{
    if (Item)
    {
        Item->Use(Location, Rotation);
        Item = nullptr;
    }
}


void UPlayerUseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

