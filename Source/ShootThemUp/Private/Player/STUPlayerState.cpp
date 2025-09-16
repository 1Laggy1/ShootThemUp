// Shoot THem Up Game. All Rights Reserved.


#include "Player/STUPlayerState.h"
#include "Net/UnrealNetwork.h"
DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All)
ASTUPlayerState::ASTUPlayerState()
{
    SetReplicates(true);
}
void ASTUPlayerState::LogInfo()
{
    /*UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);*/
}

void ASTUPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASTUPlayerState, PlayerStateNow);
    DOREPLIFETIME(ASTUPlayerState, TeamID);
    DOREPLIFETIME(ASTUPlayerState, TeamColor);
    DOREPLIFETIME(ASTUPlayerState, KillsNum);
    DOREPLIFETIME(ASTUPlayerState, DeathsNum);
}