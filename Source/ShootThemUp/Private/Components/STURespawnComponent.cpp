// Shoot THem Up Game. All Rights Reserved.


#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"
#include "Net/UnrealNetwork.h"

USTURespawnComponent::USTURespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void USTURespawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USTURespawnComponent, RespawnCountDown);

}

bool USTURespawnComponent::isRespawnInProgress() const
{
    return GetWorld() && RespawnCountDown > 0;
}

void USTURespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetOwner() || GetOwner()->GetLocalRole() != ROLE_Authority || !GetWorld())
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


