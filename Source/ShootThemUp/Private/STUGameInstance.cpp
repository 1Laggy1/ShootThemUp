// Shoot THem Up Game. All Rights Reserved.


#include "STUGameInstance.h"
#include "STUSoundFunctionLibrary.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "SteamSocketsNetDriver.h"
#include "SteamSockets/Public/SteamSocketsNetDriver.h"
#include "OnlineSubsystemSteam.h"
#include "OnlineSubsystemUtils.h"

#include <Online/OnlineSessionNames.h>


void USTUGameInstance::Init()
{
    Super::Init();
    InitSteamSocketsNetDriver();
    Subsystem = IOnlineSubsystem::Get(TEXT("STEAM"));

    if (Subsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("Steam subsystem initialized successfully"));
        SessionInterface = Subsystem->GetSessionInterface();

        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,
                                                                          &USTUGameInstance::OnCreateSessionComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USTUGameInstance::OnJoinSessionComplete);
            SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(this,
                                                                              &USTUGameInstance::OnInviteAccepted);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to init Steam subsystem!"));
    }

}

void USTUGameInstance::InitSteamSocketsNetDriver()
{
    UWorld *World = GetWorld();
    if (!World)
        return;
    if (World->GetNetDriver())
        return;

    USteamSocketsNetDriver *NetDriver =
        NewObject<USteamSocketsNetDriver>(GetTransientPackage(), USteamSocketsNetDriver::StaticClass());
    NetDriver->SetWorld(World);

    NetDriver->RecentlyDisconnectedTrackingTime = 10.0f;

    FURL ListenURL;
    ListenURL.Map = TEXT("LobbyLevel");
    ListenURL.AddOption(TEXT("listen"));

    FString Error;
    if (NetDriver->InitListen(World, ListenURL, false, Error))
    {
        World->SetNetDriver(NetDriver);
        UE_LOG(LogTemp, Log, TEXT("SteamSockets NetDriver initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to init NetDriver: %s"), *Error);
    }
}


void USTUGameInstance::CreateLobby()
{
    UNetDriver *NetDriver = GetWorld()->GetNetDriver();
    if (NetDriver)
    {
        NetDriver->RecentlyDisconnectedTrackingTime = 10.0f;
        UE_LOG(LogTemp, Log, TEXT("Set RecentlyDisconnectedTrackingTime to 10.0"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NetDriver not found!"));
    }
    if (!Subsystem || !SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Steam subsystem or session interface not valid"));
        return;
    }

    if (SessionInterface->GetNamedSession(FName(NAME_GameSession)))
    {
        SessionInterface->DestroySession(FName(NAME_GameSession));
    }
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = false;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.bUseLobbiesVoiceChatIfAvailable = false;
    SessionSettings.NumPublicConnections = 4;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bAllowInvites = true; // Enable invites
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;

    SessionSettings.Set(SETTING_GAMEMODE, FString("Lobby"), EOnlineDataAdvertisementType::ViaOnlineService);
    SessionSettings.Set(SETTING_MAPNAME, FString("LobbyLevel"), EOnlineDataAdvertisementType::ViaOnlineService);

    bool bSuccess = SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);

    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to start creating session"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Started creating Steam lobby session"));
    }
} 

void USTUGameInstance::OnInviteAccepted(const bool bWasSuccessful, int32 ControllerId,
                                        TSharedPtr<const FUniqueNetId> UserId,
                                        const FOnlineSessionSearchResult &InviteResult)
{
    if (!bWasSuccessful)
    {
        UE_LOG(LogTemp, Error, TEXT("Invite acceptance failed"));
        return;
    }

    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Session interface invalid when accepting invite"));
        return;
    }
    if (SessionInterface->GetNamedSession(NAME_GameSession) != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already in a session, skipping JoinSession"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("Invite accepted, attempting to join session..."));

    bool bJoinSuccess = SessionInterface->JoinSession(ControllerId, NAME_GameSession, InviteResult);

    if (!bJoinSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to start joining session from invite"));
    }
}
void USTUGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Session interface invalid during join"));
        return;
    }

    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to join session. Result: %d"), (int32)Result);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Successfully joined session: %s"), *SessionName.ToString());

    FString ConnectString;
    if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
    {
        UE_LOG(LogTemp, Log, TEXT("Connect string: %s"), *ConnectString);

        APlayerController *PC = GetFirstLocalPlayerController();
        if (PC)
        {
            PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No player controller found for travel"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not resolve connect string for session: %s"), *SessionName.ToString());
    }
}

void USTUGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    
        if (!bWasSuccessful)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create session!"));
            return;
        }

        UWorld *World = GetWorld();
        if (!World)
            return;

        UE_LOG(LogTemp, Log, TEXT("Session created"));

        UGameplayStatics::OpenLevel(this, TEXT("/Game/Levels/LobbyLevel?listen"), true);
}


void USTUGameInstance::OnEndSessionComplete(FName Name, bool bWasSuccessful)
{
}
void USTUGameInstance::OnCreateSession(bool Success)
{
    OnCreateSessionCompleteDelegate.Broadcast(Success);
}

void USTUGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{

}

void USTUGameInstance::ToggleVolume()
    {
    USTUSoundFunctionLibrary::ToggleSoundClassVolume(MasterSoundClass);
}
