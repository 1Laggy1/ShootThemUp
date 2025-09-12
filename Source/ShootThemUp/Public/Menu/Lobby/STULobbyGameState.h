// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STUCoreTypes.h"
#include "STULobbyGameState.generated.h"

/**
 *
 */

class ASTUBaseWeapon;
class ASTUBaseCharacter;
class USTUGameInstance;
class ASTULobbyGameMode;
UCLASS()
class SHOOTTHEMUP_API ASTULobbyGameState : public AGameStateBase
{
    GENERATED_BODY()
  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
    TArray<TSubclassOf<ASTUBaseWeapon>> WeaponsToChoose;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    FPlayerInfo DefaultPlayerInfo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    TSubclassOf<ASTUBaseCharacter> CharacterClass;

    UFUNCTION(NetMulticast, Reliable)
    void OnTeamChanged_Multicast(FTeamInfo NewTeam);
    UFUNCTION(NetMulticast, Reliable)
    void OnPlayerChanged_Multicast(FPlayerInfo PlayerChanged);
    UFUNCTION(Server, Reliable)
    void ChangeTeamName_Server(const FString &TeamName, const FString &NewTeamName);

    UFUNCTION(Server, Reliable)
    void ChangeTeamColor_Server(const FString &TeamName, const FLinearColor &Color);

    UFUNCTION(Server, Reliable)
    void ChangeWeapons_Server(TSubclassOf<ASTUBaseWeapon> WeaponToChoose, const FString &PlayerID);

    UFUNCTION(NetMulticast, Reliable)
    void SpawnAllTeams_Multicast(const TArray<FTeamInfo> &TeamsInfo);

    TMap<FString, ASTUBaseCharacter *> Characters;

    UFUNCTION()
    void OnPostLogin(APlayerController *PlayerController);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ASTUBaseCharacter> BaseCharacter;

    void AddTeamIndex()
    {
        TeamIndex = (TeamIndex + 1) % TeamNumbers;
    }

    UPROPERTY(EditDefaultsOnly)
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
    FVector GetNextPlayerStart();
    
  protected:
    void BeginPlay() override;
    void InitTeams();
    void RespawnTeam(FTeamInfo* TeamInfo);
    void RespawnPlayer(FPlayerInfo *PlayerInfo);
    void SpawnTeam(FTeamInfo& TeamInfo);
    void SpawnPlayer(FPlayerInfo *PlayerInfo, FVector Position);
    void SpawnLobbyCharacter(APlayerController *Player);
    FTeamInfo *FindTeamByTeamID(int32 TeamID);

    FPlayerInfo *FindPlayerByPlayerID(const FString &PlayerID);

    AActor *LobbyCamera;
    void SetCamera();

  private:
    int TeamIndex = 0;
    int TeamNumbers = 2;
    ASTULobbyGameMode *STULobbyGamemode;
    USTUGameInstance* STUGameInstance;
};
