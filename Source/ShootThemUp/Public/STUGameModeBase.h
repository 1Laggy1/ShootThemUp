// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"



class AAIController;
class ASTUGameStateBase;
class USTUGameInstance;
UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
  public:
    ASTUGameModeBase();
    virtual void StartPlay() override;
    AActor* GetRandomSpawnPoint(UWorld *World);
    virtual UClass *GetDefaultPawnClassForController_Implementation(AController *InController) override;
    void Killed(AController *KillerActor, AController *DiedActor);

  //  FGameData GetGameData()
  //  {
  //      return GameData;
  //  }
  //  int32 GetCurrentRound()
  //  {
  //      return CurrentRound;
  //}
  //  int32 GetRoundCountDown()
  //  {
  //      return RoundCountDown;
  //}
    void RespawnRequest(AController *Controller);

    virtual bool SetPause(APlayerController *PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;
    void PlayerConnected(APlayerController *PC);
    //FLinearColor DetermineColorByTeamID(int32 TeamID) const;
  protected:
    void ChangeState(ESTUMatchState NewState);
    void RespawnAsSpectator(AController *Controller, FVector DeathLocation, FRotator DeathRotation);
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> DefaultCharacterClass;
    FPlayerInfo *FindPlayerByPlayerID(const FString &PlayerID);
    void PostSeamlessTravel() override;
  private:
    
    virtual void PostLogin(APlayerController *NewPlayer) override;
    ASTUGameStateBase *STUGameStateBase;
    //int32 CurrentRound = 1;
    //int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;
    void SpawnBots();
    void StartRound();
    void GameTimerUpdate();
    void ResetPlayers();
    void ResetOnePlayer(AController *Controller);
    void SetPlayerColor(AActor *Character, FLinearColor TeamColor);
    /*void SetPlayerInfo(APlayerController *Controller);*/
    void LogPlayerInfo();
    void StartRespawn(AController *DiedActor);
    void GameOver();
    USTUGameInstance *STUGameInstance;
    FTimerHandle WaitingForPlayersTimerHandle;
    int PlayersNum;
    int PlayersReady = 0;
    void WaitingForPlayers();
    float WaitingTime = 30.0f;
    float BeforeStartTime = 10.0f;
    bool BeforeStart = false;
    TArray<FString> PlayersReadyIDs;
    void GetTeamsStarts();
    AActor* GetRandomTeamStart(int32 TeamID);
};
