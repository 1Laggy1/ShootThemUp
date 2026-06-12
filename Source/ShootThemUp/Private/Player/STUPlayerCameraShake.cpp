// Shoot THem Up Game. All Rights Reserved.


#include "Player/STUPlayerCameraShake.h"
#include "Camera/CameraShakeBase.h"

USTUPlayerCameraShake::USTUPlayerCameraShake()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void USTUPlayerCameraShake::BeginPlay()
{
	Super::BeginPlay();
	
}

void USTUPlayerCameraShake::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USTUPlayerCameraShake::PlayCameraShake(APlayerController *pc, TSubclassOf<UCameraShakeBase> CameraShake, float Scale)
{
    if (!pc)
        return;
	
    pc->PlayerCameraManager->StartCameraShake(CameraShake, Scale);
}