// ASTULobbyGameState.cpp
// Shoot THem Up Game. All Rights Reserved.

#include "Menu/Lobby/STULobbyGameState.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/Lobby/STULobbyGameMode.h"
#include "Menu/Lobby/STULobbyPlayerController.h"
#include "Player/STUPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Player/STUPlayerState.h"
#include "STUCoreTypes.h"
#include "STUUtils.h"
#include "STUGameInstance.h"
#include "Weapon/STUBaseWeapon.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STULobbyCharacter.h"
DECLARE_LOG_CATEGORY_CLASS(LogSTULobbyGameState, All, All);
// ----- BeginPlay / Init -----

void ASTULobbyGameState::BeginPlay()
{
    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::BeginPlay"));
    Super::BeginPlay();
    XNow = XStart;
    YNow = YStart;
    if (HasAuthority())
    {
        const auto GI = GetGameInstance();
        if (GI)
        {
            UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::BeginPlay - Got GameInstance"));
            STUGameInstance = Cast<USTUGameInstance>(GI);
        }

        const auto GM = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr;
        if (GM)
        {
            STULobbyGamemode = Cast<ASTULobbyGameMode>(GM);
            if (STULobbyGamemode)
            {
                UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::BeginPlay - Got LobbyGameMode"));
                
                STULobbyGamemode->STUOnPostLogin.AddUObject(this, &ASTULobbyGameState::OnPostLogin);
            }
        }

        InitTeams();
    }
    
    if (HasAuthority())
    {
        OnPostLogin(GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr);
    }
    
}

void ASTULobbyGameState::InitTeams()
{
    if (!STUGameInstance)
        return;

    STUGameInstance->GetTeams().Empty();

    TArray<FTeamInfo> LocalTeams;
    for (int32 i = 1; i <= TeamNumbers; ++i)
    {
        FTeamInfo TeamInfo(i, (i == 1) ? FLinearColor::Green : FLinearColor::Red);
        LocalTeams.Add(TeamInfo);
    }
    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::InitTeams - Teams initialized"));
    STUGameInstance->GetTeams() = LocalTeams;
}

// ----- OnPostLogin / Spawn all -----

void ASTULobbyGameState::OnPostLogin(APlayerController *PlayerController)
{
    if (!HasAuthority() || !PlayerController || !PlayerController->PlayerState || !STUGameInstance)
        return;

    FString CurrentPlayerID = TEXT("UnknownID");
    if (PlayerController->PlayerState->GetUniqueId().IsValid())
    {
        CurrentPlayerID = PlayerController->PlayerState->GetUniqueId()->ToString();
    }

    FPlayerInfo* ExistingPlayer = STUUtils::FindPlayerByPlayerID(CurrentPlayerID, STUGameInstance->GetTeams());
    if (ExistingPlayer)
    {
        return; 
    }

    FirstSpawnPosition = true;
    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::OnPostLogin - Player logged in"));
    FPlayerInfo NewPlayerInfo = DefaultPlayerInfo;
    if (PlayerController->PlayerState->GetUniqueId().IsValid())
    {
        NewPlayerInfo.PlayerID = PlayerController->PlayerState->GetUniqueId()->ToString();
    }
    else
    {
        NewPlayerInfo.PlayerID = TEXT("UnknownID");
    }
    NewPlayerInfo.ThisPlayerController = PlayerController;
    NewPlayerInfo.PlayerName = PlayerController->PlayerState->GetPlayerName();
    if (STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams()))
    {
        NewPlayerInfo.Color = STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->TeamColor;
        NewPlayerInfo.TeamID = STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->TeamID;
        STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->PlayersInfos.Add(NewPlayerInfo);
    }
    else
    {
        if (STUGameInstance->GetTeams().Num() > 0)
        {
            NewPlayerInfo.Color = STUUtils::FindTeamByTeamID(1, STUGameInstance->GetTeams())->TeamColor;
            NewPlayerInfo.TeamID = STUUtils::FindTeamByTeamID(1, STUGameInstance->GetTeams())->TeamID;
            STUUtils::FindTeamByTeamID(1, STUGameInstance->GetTeams())->PlayersInfos.Add(NewPlayerInfo);
        }
    }
    AddTeamID();

    SpawnAllTeams_Multicast(STUGameInstance->GetTeams());
}

void ASTULobbyGameState::SpawnAllTeams_Multicast_Implementation(const TArray<FTeamInfo> &TeamsInfo)
{
    UE_LOG(LogSTULobbyGameState, Display,
           TEXT("ASTULobbyGameState::SpawnAllTeams_Multicast_Implementation - Spawning all teams"));
    
    if (!Characters.IsEmpty())
    {
        for (auto &Pair : Characters)
        {
            if (Pair.Value)
            {
                Pair.Value->Destroy();
            }
        }
        Characters.Empty();
    }
    if (STUGameInstance)
    {
        STUGameInstance->SetTeams(TeamsInfo);
    }

    if (HasAuthority())
    {
        
        for (FTeamInfo &Team : STUGameInstance->GetTeams())
        {
            SpawnTeam(Team);
        }
    }
    
}

// ----- Spawn/Respawn helpers -----

void ASTULobbyGameState::SpawnTeam(FTeamInfo &TeamInfo)
{
    UE_LOG(LogSTULobbyGameState, Display,
           TEXT("ASTULobbyGameState::SpawnTeam - Spawning team %s with %d players"), *TeamInfo.TeamName, TeamInfo.PlayersInfos.Num());
    for (FPlayerInfo &PlayerInfo : TeamInfo.PlayersInfos)
    {
        UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::SpawnTeam - Spawning Player %s "),
               *PlayerInfo.PlayerName);

        FVector Pos = GetNextPlayerStart();
        SpawnPlayer(&PlayerInfo, Pos);
    }
}

void ASTULobbyGameState::SpawnPlayer(FPlayerInfo *PlayerInfo, FVector Position)
{
    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::SpawnPlayer - Spawning player %s at position %s"),
           *PlayerInfo->PlayerName, *Position.ToString());
    if (!PlayerInfo || !CharacterClass || !GetWorld())
        return;

    FTransform SpawnTransform(StartRotation, Position);
    if (!CharacterClass)
    {
        UE_LOG(LogSTULobbyGameState, Error, TEXT("ASTULobbyGameState::SpawnPlayer CharacterClass is NULL! Cannot spawn actor"));
        return;
    }
    ASTULobbyCharacter *Character = GetWorld()->SpawnActorDeferred<ASTULobbyCharacter>(
        CharacterClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (!Character)
        return;


    Character->SpawnInfo = *PlayerInfo;
    Character->PlayerColor = PlayerInfo->Color;
    Character->PlayerName = PlayerInfo->PlayerName;
    Character->PlayerID = PlayerInfo->PlayerID;
    UGameplayStatics::FinishSpawningActor(Character, SpawnTransform);
    /*const auto STUPlayerController = Cast<ASTUPlayerController>(PlayerInfo->ThisPlayerController);
    if (STUPlayerController)
    {
        STUPlayerController->LobbyCamera = Character->SpawnLobbyCamera();
        STUPlayerController->OnRep_SetCamera();
    }*/
    Characters.Add(PlayerInfo->PlayerID, Character);
}

bool ASTULobbyGameState::CheckSTUGameInstance()
{
    const auto GI = GetGameInstance();
    if (GI)
    {
        STUGameInstance = Cast<USTUGameInstance>(GI);
    }
    bool result = STUGameInstance ? true : false;
    return result;
}

// ----- Position generator -----

FVector ASTULobbyGameState::GetNextPlayerStart()
{
    UE_LOG(LogSTULobbyGameState, Display,
           TEXT("ASTULobbyGameState::GetNextPlayerStart - Generating next player start position"));
    if (FirstSpawnPosition)
    {
        XNow = XStart;
        YNow = YStart;
        FirstSpawnPosition = false;
        Row = 1;
        return FVector(XNow, YNow, Height);
    }

    if (Row >= RowMax)
    {
        XNow = XStart;
        YStart -= YDifference; // * (RowMax - 1);
        YNow = YStart;
        Row = 1;
        return FVector(XNow, YNow, Height);
    }
    else
    {
        ++Row;
        XNow += XDifference;
        YNow += YDifference;
    }

    return FVector(XNow, YNow, Height);
}

// ----- Multicast handlers / server ops -----

void ASTULobbyGameState::OnTeamChanged_Multicast_Implementation(FTeamInfo NewTeam)
{
    if (!CheckSTUGameInstance())
        return;
    UE_LOG(LogSTULobbyGameState, Display,
           TEXT("ASTULobbyGameState::OnTeamChanged_Multicast_Implementation - Team %s changed"), *NewTeam.TeamName);
    FTeamInfo *Team = STUUtils::FindTeamByTeamID(NewTeam.TeamID, STUGameInstance->GetTeams());
    if (Team)
    {
        *Team = NewTeam;
        RespawnTeam(Team);
    }
}

void ASTULobbyGameState::OnPlayerChanged_Multicast_Implementation(FPlayerInfo PlayerChanged)
{
    if (!CheckSTUGameInstance())
        return;
    UE_LOG(LogSTULobbyGameState, Display,
           TEXT("ASTULobbyGameState::OnPlayerChanged_Multicast_Implementation - Player %s changed"),
           *PlayerChanged.PlayerName);
    FPlayerInfo *Player = STUUtils::FindPlayerByPlayerID(PlayerChanged.PlayerID, STUGameInstance->GetTeams());
    if (Player)
    {
        *Player = PlayerChanged;
        RespawnPlayer(Player);
    }
}

void ASTULobbyGameState::ChangeTeamName_Server_Implementation(const FString &PlayerID, const FString &NewTeamName)
{
    if (!CheckSTUGameInstance())
        return;
    FPlayerInfo *Player = STUUtils::FindPlayerByPlayerID(PlayerID, STUGameInstance->GetTeams());
    if (!Player)
        return;
    FTeamInfo* CurrentTeam =
        STUGameInstance->GetTeams().FindByPredicate([&](const FTeamInfo &T) { return T.TeamID == Player->TeamID; });
    if (CurrentTeam)
    {
        CurrentTeam->TeamName = NewTeamName;
        OnTeamChanged_Multicast(*CurrentTeam);
    }
}

void ASTULobbyGameState::ChangeTeamColor_Server_Implementation(const FString &PlayerID, const FLinearColor &Color)
{
    if (!CheckSTUGameInstance())
        return;
    FPlayerInfo *Player = STUUtils::FindPlayerByPlayerID(PlayerID, STUGameInstance->GetTeams());
    if (!Player)
        return;
    FTeamInfo *CurrentTeam =
        STUGameInstance->GetTeams().FindByPredicate([&](const FTeamInfo &T) { return T.TeamID == Player->TeamID; });
    if (CurrentTeam)
    {
        CurrentTeam->TeamColor = Color;
        for (FPlayerInfo& PlayerInfo : CurrentTeam->PlayersInfos)
        {
            PlayerInfo.Color = Color;
        }
        OnTeamChanged_Multicast(*CurrentTeam);
    }
}

void ASTULobbyGameState::ChangeWeapons_Server_Implementation(TSubclassOf<ASTUBaseWeapon> WeaponToChoose,
                                                             const FString &PlayerID)
{
    if (!CheckSTUGameInstance())
        return;
    FPlayerInfo *Player = STUUtils::FindPlayerByPlayerID(PlayerID, STUGameInstance->GetTeams());
    if (Player && WeaponToChoose)
    {
        if (WeaponsToChoose.Contains(WeaponToChoose))
        {
            Player->WeaponClass = WeaponToChoose;
            OnPlayerChanged_Multicast(*Player);
        }
    }
}

void ASTULobbyGameState::ChangeAbility_Server_Implementation(TSubclassOf<USTUPlayerAbilityUseComponent> AbilityToChoose,
                                                             const FString &PlayerID)
{
    if (!CheckSTUGameInstance())
        return;
    FPlayerInfo *Player = STUUtils::FindPlayerByPlayerID(PlayerID, STUGameInstance->GetTeams());
    if (Player && AbilityToChoose)
    {
        if (AbilitiesToChoose.Contains(AbilityToChoose))
        {
            Player->AbilityClass = TSoftClassPtr<USTUPlayerAbilityUseComponent>(AbilityToChoose.Get());
            OnPlayerChanged_Multicast(*Player);
        }
    }
}

// ----- Respawn helpers -----

void ASTULobbyGameState::RespawnTeam(FTeamInfo *TeamInfo)
{
    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::RespawnTeam - Respawning team %s"),
           TeamInfo ? *TeamInfo->TeamName : TEXT("Invalid"));
    if (!TeamInfo)
        return;

    for (FPlayerInfo &PlayerInfo : TeamInfo->PlayersInfos)
    {
        RespawnPlayer(&PlayerInfo);
    }
}

void ASTULobbyGameState::RespawnPlayer(FPlayerInfo *PlayerInfo)
{
    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::RespawnPlayer - Respawning player %s"),
           PlayerInfo ? *PlayerInfo->PlayerName : TEXT("Invalid"));
    if (!PlayerInfo)
        return;

    ASTUBaseCharacter **FoundPtr = Characters.Find(PlayerInfo->PlayerID);
    ASTUBaseCharacter *Character = FoundPtr ? *FoundPtr : nullptr;
    if (!Character)
        return;

    FVector PreviousPosition = Character->GetActorLocation();

    Character->Destroy();

    FVector StartPosition = PreviousPosition;
    SpawnPlayer(PlayerInfo, StartPosition);
}

void ASTULobbyGameState::SpawnFakePlayers(int32 Count)
{
    if (!HasAuthority() || !STUGameInstance)
        return;

    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::SpawnFakePlayers - Generating %d fake players"),
           Count);

    for (int32 i = 0; i < Count; ++i)
    {
        FPlayerInfo FakePlayerInfo = DefaultPlayerInfo;

        FakePlayerInfo.PlayerID = FString::Printf(TEXT("FakeBotID_%d"), FMath::RandRange(10000, 99999));
        FakePlayerInfo.PlayerName = FString::Printf(TEXT("TestBot_%d"), i + 1);

        if (STUGameInstance->GetTeams().IsValidIndex(TeamID))
        {
            FakePlayerInfo.Color = STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->TeamColor;
            FakePlayerInfo.TeamID = STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->TeamID;
            STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->PlayersInfos.Add(FakePlayerInfo);
        }
        else if (STUGameInstance->GetTeams().Num() > 0)
        {
            FakePlayerInfo.Color = STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->TeamColor;
            FakePlayerInfo.TeamID = STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->TeamID;
            STUUtils::FindTeamByTeamID(TeamID, STUGameInstance->GetTeams())->PlayersInfos.Add(FakePlayerInfo);
        }

        AddTeamID();
    }


    FirstSpawnPosition = true;

    SpawnAllTeams_Multicast(STUGameInstance->GetTeams());
}
