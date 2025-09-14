// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STUCoreTypes.h"
#include "STUGameStateBase.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStatistics, FMatchStatistics *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, ESTUMatchState);

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
  public:
    void BeginPlay() override;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    FGameData GameData;
    UPROPERTY(Replicated)
    int32 CurrentRound = 0;
    UPROPERTY(Replicated)
    int32 RoundCountDown = 0;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
    UPROPERTY(Replicated)
    TArray<FTeamInfo> TeamsStats;
    UFUNCTION(Server, Reliable)
    void PlayerConnected(APlayerController *PC);

    
    /*UPROPERTY(Replicated)
    FMatchStatistics Statistics;
    UFUNCTION()
    void OnRep_MatchStateChanged();
    FMatchStatistics GetMatchStatistics() {
        return Statistics;
    };

    UPROPERTY(ReplicatedUsing = MatchStatisticsBroadcast)
    FMatchStatistics MatchStatistics;*/
    /*UFUNCTION()
    void MatchStatisticsBroadcast()
    {
        OnMatchStatistics.Broadcast(&MatchStatistics);
    }*/
    //FOnMatchStatistics OnMatchStatistics;
    FOnMatchStateChanged OnMatchStateChanged;

  public:
    UFUNCTION(NetMulticast, Reliable)
    void SetPlayerColorMulticast(AActor *Player, FLinearColor TeamColor);
    UFUNCTION(NetMulticast, Reliable)
    void ResetOnePlayerMulticast(AActor *DiedActor, AActor *Spawn);
    FGameData GetGameData()
    {
        return GameData;
    }
    void SetGameData(FGameData GameDataServer, TArray<FTeamInfo> TeamsInfo)
    {
        GameData = GameDataServer;
        TeamsStats = TeamsInfo;
    }
    int32 GetCurrentRound()
    {
        return CurrentRound;
    }
    int32 GetRoundCountDown()
    {
        return RoundCountDown;
    }
    void SetMatchState(ESTUMatchState State)
    {
        MatchState = State;
    }
  protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
    
    
};
