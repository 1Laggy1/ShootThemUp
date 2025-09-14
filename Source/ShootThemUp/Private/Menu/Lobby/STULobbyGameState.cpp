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

    STUGameInstance->Teams.Empty();

    TArray<FTeamInfo> LocalTeams;
    for (int32 i = 1; i <= TeamNumbers; ++i)
    {
        FTeamInfo TeamInfo(i, (i == 1) ? FLinearColor::Green : FLinearColor::Red);
        LocalTeams.Add(TeamInfo);
    }
    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::InitTeams - Teams initialized"));
    STUGameInstance->Teams = LocalTeams;
}

// ----- OnPostLogin / Spawn all -----

void ASTULobbyGameState::OnPostLogin(APlayerController *PlayerController)
{
    if (!HasAuthority() || !PlayerController || !PlayerController->PlayerState || !STUGameInstance)
        return;
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
    if (STUGameInstance->Teams.IsValidIndex(TeamIndex))
    {
        NewPlayerInfo.Color = STUGameInstance->Teams[TeamIndex].TeamColor;
        NewPlayerInfo.TeamID = STUGameInstance->Teams[TeamIndex].TeamID;
        STUGameInstance->Teams[TeamIndex].PlayersInfos.Add(NewPlayerInfo);
    }
    else
    {
        if (STUGameInstance->Teams.Num() > 0)
        {
            NewPlayerInfo.Color = STUGameInstance->Teams[0].TeamColor;
            NewPlayerInfo.TeamID = STUGameInstance->Teams[0].TeamID;
            STUGameInstance->Teams[0].PlayersInfos.Add(NewPlayerInfo);
        }
    }
    AddTeamIndex();

    SpawnAllTeams_Multicast(STUGameInstance->Teams);
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
        STUGameInstance->Teams = TeamsInfo;
    }

    if (HasAuthority())
    {
        
        for (FTeamInfo &Team : STUGameInstance->Teams)
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
    UGameplayStatics::FinishSpawningActor(Character, SpawnTransform);
    const auto STUPlayerController = Cast<ASTUPlayerController>(PlayerInfo->ThisPlayerController);
    if (STUPlayerController)
    {
        STUPlayerController->LobbyCamera = Character->SpawnLobbyCamera();
        STUPlayerController->OnRep_SetCamera();
    }
    Characters.Add(PlayerInfo->PlayerID, Character);
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
        //YNow += YDifference;
    }

    return FVector(XNow, YNow, Height);
}

// ----- Multicast handlers / server ops -----

void ASTULobbyGameState::OnTeamChanged_Multicast_Implementation(FTeamInfo NewTeam)
{
    UE_LOG(LogSTULobbyGameState, Display,
           TEXT("ASTULobbyGameState::OnTeamChanged_Multicast_Implementation - Team %s changed"), *NewTeam.TeamName);
    FTeamInfo *Team = STUUtils::FindTeamByTeamID(NewTeam.TeamID, STUGameInstance);
    if (Team)
    {
        *Team = NewTeam;
        //RespawnTeam(Team);
    }
}

void ASTULobbyGameState::OnPlayerChanged_Multicast_Implementation(FPlayerInfo PlayerChanged)
{
    UE_LOG(LogSTULobbyGameState, Display,
           TEXT("ASTULobbyGameState::OnPlayerChanged_Multicast_Implementation - Player %s changed"),
           *PlayerChanged.PlayerName);
    FPlayerInfo *Player = STUUtils::FindPlayerByPlayerID(PlayerChanged.PlayerID, STUGameInstance->Teams);
    if (Player)
    {
        *Player = PlayerChanged;
        //RespawnPlayer(Player);
    }
}

void ASTULobbyGameState::ChangeTeamName_Server_Implementation(const FString &TeamName, const FString &NewTeamName)
{
    FTeamInfo *Team = STUGameInstance->Teams.FindByPredicate([&](const FTeamInfo &T) { return T.TeamName == TeamName; });
    if (Team)
    {
        Team->TeamName = NewTeamName;
        OnTeamChanged_Multicast(*Team);
    }
}

void ASTULobbyGameState::ChangeTeamColor_Server_Implementation(const FString &TeamName, const FLinearColor &Color)
{
    FTeamInfo *Team =
        STUGameInstance->Teams.FindByPredicate([&](const FTeamInfo &T) { return T.TeamName == TeamName; });
    if (Team)
    {
        Team->TeamColor = Color;
        OnTeamChanged_Multicast(*Team);
    }
}

void ASTULobbyGameState::ChangeWeapons_Server_Implementation(TSubclassOf<ASTUBaseWeapon> WeaponToChoose,
                                                             const FString &PlayerID)
{
    FPlayerInfo *Player = STUUtils::FindPlayerByPlayerID(PlayerID, STUGameInstance->Teams);
    if (Player && WeaponToChoose)
    {
        if (WeaponsToChoose.Contains(WeaponToChoose))
        {
            Player->WeaponClass = WeaponToChoose;
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
    /*FTransform NewTransform(StartRotation, StartPosition);
    if (!CharacterClass)
    {
        UE_LOG(LogSTULobbyGameState, Error,
               TEXT("ASTULobbyGameState::RespawnPlayer CharacterClass is NULL! Cannot spawn actor"));
        return;
    }
    ASTUBaseCharacter *PlayerCharacter = GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(
        CharacterClass, NewTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (PlayerCharacter)
    {
        PlayerCharacter->SpawnInfo = DefaultPlayerInfo;
        UGameplayStatics::FinishSpawningActor(PlayerCharacter, NewTransform);
    }*/
}
// ----- Spawn lobby character -----
//
//void ASTULobbyGameState::SpawnLobbyCharacter(APlayerController *Player)
//{
//    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::SpawnLobbyCharacter - Spawning lobby character"));
//    if (!Player)
//        return;
//
//    FVector StartPosition = GetNextPlayerStart();
//    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::SpawnLobbyCharacter - Start position: %s"),
//           *StartPosition.ToString());
//    FTransform NewTransform(StartRotation, StartPosition);
//    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::SpawnLobbyCharacter - Rotation: %s"),
//           *StartRotation.ToString());
//    if (!CharacterClass)
//    {
//        UE_LOG(LogSTULobbyGameState, Error,
//               TEXT(" ASTULobbyGameState::SpawnLobbyCharacter CharacterClass is NULL! Cannot spawn actor"));
//        return;
//    }
//    ASTULobbyCharacter *PlayerCharacter = GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(
//        CharacterClass, NewTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
//    if (PlayerCharacter)
//    {
//        PlayerCharacter->SpawnInfo = DefaultPlayerInfo;
//        UGameplayStatics::FinishSpawningActor(PlayerCharacter, NewTransform);
//
//        const auto STUPlayerController =
//            Cast<ASTULobbyPlayerController>(PlayerCharacter->SpawnInfo.ThisPlayerController);
//        if (STUPlayerController)
//        {
//            STUPlayerController->SetCamera(StartPosition + RelatedCameraPosition, RelatedCameraRotation);
//        }
//
//        
//    }
//}
//void ASTULobbyGameState::SetCamera(FVector PlayerLocation)
//{
//    UE_LOG(LogSTULobbyGameState, Display, TEXT("ASTULobbyGameState::SetCamera - Setting lobby camera"));
//    TArray<AActor *> Cameras;
//    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
//    LobbyCamera = Cast<ACameraActor>(Cameras[0]);
//    APlayerController *PC = GetWorld()->GetFirstPlayerController();
//    
//    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//    {
//        APlayerController *PC = It->Get();
//        if (PC)
//        {
//            UE_LOG(LogSTULobbyGameState, Display,
//                   TEXT("ASTULobbyGameState::SetCamera - Setting view target for player"));
//            
//            PC->SetViewTarget(LobbyCamera);
//        }
//    }
//}
// ----- Find helpers -----

