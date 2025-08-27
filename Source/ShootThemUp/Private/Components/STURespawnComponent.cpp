// Shoot THem Up Game. All Rights Reserved.


#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


bool USTURespawnComponent::isRespawnInProgress() const
{
    return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

void USTURespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetWorld())
        return;
    RespawnCountDown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f,
                                           true);

}

void USTURespawnComponent::RespawnTimerUpdate()
{
    if (--RespawnCountDown == 0)
    {
        if (!GetWorld())
            return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
        const auto Gamemode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (!Gamemode)
            return;
        Gamemode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}


