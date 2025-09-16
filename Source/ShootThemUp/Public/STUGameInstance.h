// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "STUCoreTypes.h"
#include <ThirdParty/Steamworks/Steamv157/sdk/public/steam/steamclientpublic.h>
#include "STUGameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTUOnCreateSessionComplete, bool, Success);
class USoundClass;

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
    GENERATED_BODY()
  public:
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer")
    FSTUOnCreateSessionComplete OnCreateSessionCompleteDelegate;
    virtual void Init() override;
    IOnlineSubsystem *GetOnlineSubsystem()
    {
        return Subsystem;
    }
    FLevelData GetStartupLevel()
    {
        return StartupLevel;
    }
    void SetStartupLevel(const FLevelData &Data)
    {
        StartupLevel = Data;
    }
    TArray<FLevelData> GetLevelsData() const
    {
        return LevelsData;
    }
    FName GetMainMenuLevelName()
    {
        return MainMenuLevelName;
    }
    FName GetLobbyLevelName()
    {
        return LobbyLevelName;
    }
    void ToggleVolume();
    void CreateLobby();
    void InitSteamSocketsNetDriver();
    void OnInviteAccepted(const bool bWasSuccessful, int32 ControllerId, TSharedPtr<const FUniqueNetId> UserId,
                          const FOnlineSessionSearchResult &InviteResult);
    
    TArray<FTeamInfo> Teams;
    
  protected:
    IOnlineSessionPtr SessionInterface;
    IOnlineSubsystem *Subsystem;

    UFUNCTION(BlueprintCallable, Category = "Game")
    void OnCreateSession(bool Success);

    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void OnEndSessionComplete(FName Name, bool bWasSuccessful);

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FLevelData> LevelsData;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName StartupLevelName = NAME_None;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MainMenuLevelName = NAME_None;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName LobbyLevelName = NAME_None;
    FLevelData StartupLevel;

    

  private:
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundClass *MasterSoundClass;
};
