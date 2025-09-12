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
        TeamID = ID;
    }
    int32 GetTeamID() const
    {
        return TeamID;
    }
    void SetTeamColor(const FLinearColor &Color)
    {
        TeamColor = Color;
    }
    FLinearColor GetTeamColor() const
    {
        return TeamColor;
    }
    void AddKill()
    {
        KillsNum++;
    }
    void RemoveKill()
    {
        KillsNum--;
    }
    int32 GetKillsNum()
    {
        return KillsNum;
    }
    int32 GetDeathsNum()
    {
        return DeathsNum;
    }
    void AddDeath()
    {
        DeathsNum++;
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
    UPROPERTY(Replicated)
    STUPlayerStateEnum PlayerStateNow = STUPlayerStateEnum::None;

  private:
    UPROPERTY(Replicated)
    int32 TeamID;
    UPROPERTY(Replicated)
    FLinearColor TeamColor;
    UPROPERTY(Replicated)
    int32 KillsNum = 0;
    UPROPERTY(Replicated)
    int32 DeathsNum = 0;
    
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
