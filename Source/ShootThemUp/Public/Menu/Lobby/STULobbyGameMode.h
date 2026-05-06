// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "Menu/Lobby/STULobbyPlayerStart.h"
#include "STULobbyGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSTUOnPostLogin, APlayerController*)
class ASTUBaseCharacter;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTULobbyGameMode : public AGameModeBase
{
    GENERATED_BODY()
    ASTULobbyGameMode();

  public:
    void StartPlay() override;
    FSTUOnPostLogin STUOnPostLogin;
    
    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //TArray<ASTULobbyPlayerStart *> PlayerStarts;
    
  protected:
    
  private:
    /*UPROPERTY(EditDefaultsOnly)
    float XStart;
    UPROPERTY(EditDefaultsOnly)
    float YStart;
    UPROPERTY(EditDefaultsOnly)
    float XDifference;
    UPROPERTY(EditDefaultsOnly)
    float YDifference;
    float XNow = XStart;
    float YNow = YStart;
    int Row = 1;
    UPROPERTY(EditDefaultsOnly)
    int RowMax = 3;
    UPROPERTY(EditDefaultsOnly)
    FRotator StartRotation;
    UPROPERTY(EditDefaultsOnly)
    float Height;
    bool FirstSpawnPosition = true;
    void BeginFindPlayerStarts();
    FVector GetNextPlayerStart();*/
    virtual void PostLogin(APlayerController *NewPlayer) override;
    void SpawnLobbyCharacter(APlayerController *Player);
    void HandleStartingNewPlayer_Implementation(APlayerController *NewPlayer) override;
};

