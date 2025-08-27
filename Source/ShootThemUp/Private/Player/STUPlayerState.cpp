// Shoot THem Up Game. All Rights Reserved.


#include "Player/STUPlayerState.h"
DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All)
void ASTUPlayerState::LogInfo()
{
    UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum)
}