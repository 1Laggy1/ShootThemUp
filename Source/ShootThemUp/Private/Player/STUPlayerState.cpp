// Shoot THem Up Game. All Rights Reserved.


#include "Player/STUPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "STUUtils.h"
#include "STUGameStateBase.h"
DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All)
ASTUPlayerState::ASTUPlayerState()
{
    SetReplicates(true);
}
void ASTUPlayerState::LogInfo()
{
    /*UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);*/
    
}

void ASTUPlayerState::SetPlayerStats_Implementation(const FString &SetPlayerID)
{
    if (!GetWorld() || !GetWorld()->GetGameState<ASTUGameStateBase>())
        return;
    ASTUGameStateBase *GameStateBase = GetWorld()->GetGameState<ASTUGameStateBase>();
    Stats = STUUtils::FindPlayerByPlayerID(SetPlayerID, GameStateBase->TeamsStats);
    UE_LOG(LogSTUPlayerState, Display, TEXT("SetPlayerStats_Implementation called for %s"), *SetPlayerID);
}

void ASTUPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASTUPlayerState, PlayerStateNow);
    /*DOREPLIFETIME(ASTUPlayerState, TeamID);
    DOREPLIFETIME(ASTUPlayerState, TeamColor);
    DOREPLIFETIME(ASTUPlayerState, KillsNum);
    DOREPLIFETIME(ASTUPlayerState, DeathsNum);*/
}