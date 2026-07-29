// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STUCoreTypes.h"
#include "STUGameStateBase.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStatistics, FMatchStatistics *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, ESTUMatchState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerChanged, int32);

/**
 * 
 */
class ASTUBaseCharacter;
class ASTUPlayerController;
UCLASS()
class SHOOTTHEMUP_API ASTUGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
  public:
    void BeginPlay() override;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    FGameData GameData;
    /*UPROPERTY(Replicated)
    int32 CurrentRound = 0;*/
    UPROPERTY(Replicated)
    int32 GameCountDown = 0;
    UPROPERTY(ReplicatedUsing = OnRep_TimerChanged)
    int32 BetweenGoalsCountDown = 0;
    UPROPERTY(Replicated)
    int32 AfterGoalCountDown = 0;
    UPROPERTY(ReplicatedUsing = OnRep_MatchState, EditAnywhere, BlueprintReadWrite)
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
    UFUNCTION()
    void OnRep_TeamsStats();
    // UFUNCTION(Server, Reliable)
    // void PlayerConnected(APlayerController *PC);
    UPROPERTY(ReplicatedUsing = OnRep_TimerChanged)
    float WaitingTimeNow = 30.0f;
    TArray<FTeamInfo>& GetTeams() { return TeamsStats; }
    void SetTeams(const TArray<FTeamInfo>& NewTeams);

    void WaitForReplicate();
    FTimerHandle WaitForReplicateTimerHandle;

    bool SetIntroCameraView(ASTUPlayerController *PC);
    bool SetOutlineColors(ASTUPlayerController *PC);

    FOnMatchStateChanged OnMatchStateChanged;
    FOnTimerChanged OnTimerChanged;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialParameterCollection* OutlineColorsMPC;
    bool isOutlineColorsChanged = false;

    UPROPERTY(Replicated)
    AActor* IntroCamera;

  public:
    /*UFUNCTION(NetMulticast, Reliable)
    void SetPlayerColorMulticast(AActor *Player, FLinearColor TeamColor);*/
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
        GameCountDown = GameData.GameTime;
    }
    //int32 GetCurrentRound()
    //{
    //    return CurrentRound;
    //}
    int32 GetGameCountDown()
    {
        return GameCountDown;
    }
    void SetMatchState(ESTUMatchState State)
    {
        MatchState = State;
    }
    UFUNCTION()
    void OnRep_MatchState()
    {
        OnMatchStateChanged.Broadcast(MatchState);
    }
    UFUNCTION()
    void OnRep_TimerChanged();

    
  protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
  private:
    UPROPERTY(ReplicatedUsing = OnRep_TeamsStats)
    TArray<FTeamInfo> TeamsStats;
    
    
};
