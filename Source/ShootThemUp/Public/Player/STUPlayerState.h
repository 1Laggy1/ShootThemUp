// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUCoreTypes.h"
#include "STUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()
  public:
    ASTUPlayerState();
    void SetTeamID(int32 ID)
    {
        if (!Stats)
            return;
        Stats->TeamID = ID;
    }
    int32 GetTeamID() const
    {
        if (!Stats)
            return 0;
        return Stats->TeamID;
    }
    void SetTeamColor(const FLinearColor &Color)
    {
        if (!Stats)
            return;
        Stats->Color = Color;
    }
    FLinearColor GetTeamColor() const
    {
        if (!Stats)
            return FLinearColor::White;
        return Stats->Color;
    }
    void AddKill()
    {
        if (!Stats)
            return;
        Stats->Kills++;
    }
    void RemoveKill()
    {
        if (!Stats)
            return;
        Stats->Kills--;
    }
    int32 GetKillsNum()
    {
        if (!Stats)
            return 0;
        return Stats->Kills;
    }
    int32 GetDeathsNum()
    {
        if (!Stats)
            return 0;
        return Stats->Deaths;
    }
    void AddDeath()
    {
        if (!Stats)
            return;
        Stats->Deaths++;
    }
    void LogInfo();
    STUPlayerStateEnum GetSTUPlayerState()
    {
        return PlayerStateNow;
    }
    void SetSTUPlayerState(STUPlayerStateEnum State)
    {
        PlayerStateNow = State;
    }
    UFUNCTION(Client, Reliable)
    void SetPlayerStats(const FString& SetPlayerID);
    UPROPERTY(Replicated)
    STUPlayerStateEnum PlayerStateNow = STUPlayerStateEnum::None;
    bool LoadedAndNotifiedServer = false;
  private:
    /*UPROPERTY(Replicated)
    int32 TeamID;
    UPROPERTY(Replicated)
    FLinearColor TeamColor;
    UPROPERTY(Replicated)
    int32 KillsNum = 0;
    UPROPERTY(Replicated)
    int32 DeathsNum = 0;*/
    FPlayerInfo *Stats;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
