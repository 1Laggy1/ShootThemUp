// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STUCoreTypes.h"
#include "STUGameStateBase.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
  public:
    FOnMatchStateChangeSignature OnMatchStateChanged;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    FGameData GameData;
    UPROPERTY(Replicated)
    int32 CurrentRound = 0;
    UPROPERTY(Replicated)
    int32 RoundCountDown = 0;
    UPROPERTY(ReplicatedUsing = OnRep_MatchStateChanged, EditAnywhere, BlueprintReadWrite)
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
    UFUNCTION()
    void OnRep_MatchStateChanged();

  public:
    UFUNCTION(NetMulticast, Reliable)
    void SetPlayerColorMulticast(AActor *Player, FLinearColor TeamColor);
    UFUNCTION(NetMulticast, Reliable)
    void ResetOnePlayerMulticast(AActor *DiedActor, AActor *Spawn);
    FGameData GetGameData()
    {
        return GameData;
    }
    void SetGameData(FGameData GameDataServer)
    {
        GameData = GameDataServer;
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
