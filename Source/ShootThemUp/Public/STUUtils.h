#pragma once
#include "Player/STUPlayerState.h"
#include "STUGameInstance.h"
#include "STUCoreTypes.h"

class STUUtils
{
  public:
    template <typename T> static T *GetSTUPlayerComponent(AActor *PlayerPawn)
    {
        if (!PlayerPawn)
            return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
    bool static AreEnemies(AController* Controller1, AController* Controller2) 
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2)
            return false;

        const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
        const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

        return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
    }
    int32 static GetRandomIDRange()
    {
        return FMath::RandRange(0, 99999);
    }
    AController static *GetInstigatorControllerFromDamageCauser(AActor *DamageCauser)
    {
        if (!DamageCauser)
            return nullptr;

        if (APawn *Pawn = Cast<APawn>(DamageCauser))
        {
            return Pawn->GetController();
        }

        if (AController *InstCtrl = DamageCauser->GetInstigatorController())
        {
            return InstCtrl;
        }

        if (AActor *OwnerCauser = DamageCauser->GetOwner())
        {
            if (APawn *OwnerPawn = Cast<APawn>(OwnerCauser))
                return OwnerPawn->GetController();

            if (AController *OwnerCtrl = OwnerCauser->GetInstigatorController())
                return OwnerCtrl;
        }

        return nullptr;
    }
    FPlayerStats static GetPlayerStatsFromPlayerState(ASTUPlayerState* PlayerState)
    {
        FPlayerStats NewPlayerStats;
        if (!PlayerState)
            return NewPlayerStats;
        NewPlayerStats.PlayerName = PlayerState->GetPlayerName();
        NewPlayerStats.Kills = PlayerState->GetKillsNum();
        NewPlayerStats.Deaths = PlayerState->GetDeathsNum();
        NewPlayerStats.TeamID = PlayerState->GetTeamID();
        NewPlayerStats.TeamColor = PlayerState->GetTeamColor();
        return NewPlayerStats;
    }
    FStatRowInfo static GetStatRowInfoFromFPlayerStats(FPlayerStats &PlayerStats)
    {
        FStatRowInfo RowInfo;
        RowInfo.PlayerNameText = PlayerStats.PlayerName;
        RowInfo.KillsText = FString::FromInt(PlayerStats.Kills);
        RowInfo.DeathsText = FString::FromInt(PlayerStats.Deaths);
        RowInfo.TeamText = FString::FromInt(PlayerStats.TeamID);
        RowInfo.TeamColor = PlayerStats.TeamColor;
        return RowInfo;
    }
    FTeamInfo static *FindTeamByTeamID(int32 TeamID, USTUGameInstance *GameInstance)
    {
        return GameInstance->Teams.FindByPredicate([TeamID](const FTeamInfo &Team) { return Team.TeamID == TeamID; });
    }

    static FPlayerInfo *FindPlayerByPlayerID(const FString &PlayerID, TArray<FTeamInfo> &Teams)
    {
        if (Teams.IsEmpty())
            return nullptr;
        for (FTeamInfo &Team : Teams)
        {
            FPlayerInfo *Player =
                Team.PlayersInfos.FindByPredicate([&](const FPlayerInfo &P) { return P.PlayerID == PlayerID; });
            if (Player)
            {
                return Player;
            }
        }
        return nullptr;
    }
};
