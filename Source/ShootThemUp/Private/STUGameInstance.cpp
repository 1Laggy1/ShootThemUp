// Shoot THem Up Game. All Rights Reserved.


#include "STUGameInstance.h"
#include "STUSoundFunctionLibrary.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"


#include <Online/OnlineSessionNames.h>


void USTUGameInstance::Init()
{
    Super::Init();
    Subsystem = IOnlineSubsystem::Get();
    
if (Subsystem)
    {
        SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            /*SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,
                                                                          &USTUGameInstance::OnCreateSessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,
                                                                         &USTUGameInstance::OnFindSessionsComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USTUGameInstance::OnJoinSessionComplete);
            SessionInterface->OnEndSessionCompleteDelegates.AddUObject(
                this, &USTUGameInstance::OnEndSessionComplete);
            SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(
                this, &USTUGameInstance::OnSessionUserInviteAccepted);*/

        }
    }
}
void USTUGameInstance::OnEndSessionComplete(FName Name, bool bWasSuccessful)
{
}
void USTUGameInstance::OnCreateSession(bool Success)
{
    OnCreateSessionCompleteDelegate.Broadcast(Success);
}
void USTUGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    //if (bWasSuccessful && SessionInterface.IsValid())
    //{
    //    // Register the local player with the session
    //    SessionInterface->RegisterPlayer(SessionName, *GetFirstGamePlayer()->GetPreferredUniqueNetId(), false);
    //}
}
void USTUGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{

}
void USTUGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    //UE_LOG(LogTemp, Log, TEXT("Join session result: %d"), (int32)Result);

    //if (!SessionInterface.IsValid())
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Session interface invalid"));
    //    return;
    //}

    //if (Result != EOnJoinSessionCompleteResult::Success)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Failed to join session"));
    //    return;
    //}

    //FString ConnectString;
    //if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
    //{
    //    UE_LOG(LogTemp, Log, TEXT("Original connect string: %s"), *ConnectString);

    //    // Clean up the connect string (remove port if present)
    //    if (ConnectString.Contains(":"))
    //    {
    //        ConnectString = ConnectString.Left(ConnectString.Find(":"));
    //    }

    //    // Add the lobby level to the connect string
    //    FString LobbyLevelNameCurrent = GetLobbyLevelName().ToString();
    //    ConnectString = FString::Printf(TEXT("%s/Game/Levels/%s"), *ConnectString, *LobbyLevelNameCurrent);

    //    UE_LOG(LogTemp, Log, TEXT("Final connect string: %s"), *ConnectString);

    //    APlayerController *PC = GetFirstLocalPlayerController();
    //    if (PC)
    //    {
    //        PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Error, TEXT("No player controller found"));
    //    }
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Failed to get connect string"));
    //}
}

void USTUGameInstance::ToggleVolume()
    {
    USTUSoundFunctionLibrary::ToggleSoundClassVolume(MasterSoundClass);
}


void USTUGameInstance::CreateSession()
{
    
}


void USTUGameInstance::CloseSession()

{
    if (!SessionInterface.IsValid())
        return;

    auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession != nullptr)
    {
        SessionInterface->DestroySession(NAME_GameSession);
    }
}

void USTUGameInstance::OnSessionUserInviteAccepted(
    const bool bWasSuccessful, int32 ControllerId, TSharedPtr<const FUniqueNetId> UserId,
    const FOnlineSessionSearchResult &InviteResult)
{
    /*if (bWasSuccessful && SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Accepting invite to session"));

        
        CloseSession();

        
        SessionInterface->JoinSession(ControllerId, NAME_GameSession, InviteResult);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to accept invite: bWasSuccessful=%s"),
               bWasSuccessful ? TEXT("true") : TEXT("false"));
    }*/
}
