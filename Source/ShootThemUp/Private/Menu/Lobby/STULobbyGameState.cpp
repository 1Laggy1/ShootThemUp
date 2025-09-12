// ASTULobbyGameState.cpp
// Shoot THem Up Game. All Rights Reserved.

#include "Menu/Lobby/STULobbyGameState.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/Lobby/STULobbyGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Player/STUPlayerState.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.h"
#include "Weapon/STUBaseWeapon.h"
#include "Player/STUBaseCharacter.h"
// ----- BeginPlay / Init -----

void ASTULobbyGameState::BeginPlay()
{
    Super::BeginPlay();
    XNow = XStart;
    YNow = YStart;
    if (HasAuthority())
    {
        const auto GI = GetGameInstance();
        if (GI)
        {
            STUGameInstance = Cast<USTUGameInstance>(GI);
        }

        const auto GM = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr;
        if (GM)
        {
            STULobbyGamemode = Cast<ASTULobbyGameMode>(GM);
            if (STULobbyGamemode)
            {
                STULobbyGamemode->STUOnPostLogin.AddUObject(this, &ASTULobbyGameState::OnPostLogin);
            }
        }

        InitTeams();
    }
    SetCamera();
}

void ASTULobbyGameState::InitTeams()
{
    if (!STUGameInstance)
        return;

    STUGameInstance->Teams.Empty();

    TArray<FTeamInfo> LocalTeams;
    for (int32 i = 1; i <= TeamNumbers; ++i)
    {
        FTeamInfo TeamInfo(i, (i == 1) ? FLinearColor::White : FLinearColor::Red);
        LocalTeams.Add(TeamInfo);
    }

    STUGameInstance->Teams = LocalTeams;
}

// ----- OnPostLogin / Spawn all -----

void ASTULobbyGameState::OnPostLogin(APlayerController *PlayerController)
{
    if (!HasAuthority() || !PlayerController || !PlayerController->PlayerState || !STUGameInstance)
        return;

    FPlayerInfo NewPlayerInfo = DefaultPlayerInfo;
    if (PlayerController->PlayerState->GetUniqueId().IsValid())
    {
        NewPlayerInfo.PlayerID = PlayerController->PlayerState->GetUniqueId()->ToString();
    }
    else
    {
        NewPlayerInfo.PlayerID = TEXT("UnknownID");
    }

    NewPlayerInfo.PlayerName = PlayerController->PlayerState->GetPlayerName();
    // безпечна перевірка індексу TeamIndex
    if (STUGameInstance->Teams.IsValidIndex(TeamIndex))
    {
        NewPlayerInfo.Color = STUGameInstance->Teams[TeamIndex].TeamColor;
        NewPlayerInfo.TeamID = STUGameInstance->Teams[TeamIndex].TeamID;
        STUGameInstance->Teams[TeamIndex].PlayersInfos.Add(NewPlayerInfo);
    }
    else
    {
        // якщо індекс некоректний — поставити в першу команду
        if (STUGameInstance->Teams.Num() > 0)
        {
            NewPlayerInfo.Color = STUGameInstance->Teams[0].TeamColor;
            NewPlayerInfo.TeamID = STUGameInstance->Teams[0].TeamID;
            STUGameInstance->Teams[0].PlayersInfos.Add(NewPlayerInfo);
        }
    }
    AddTeamIndex();

    // Виклик серверної функції (у header SpawnAllTeams_Multicast — оголошено як Server)
    SpawnAllTeams_Multicast(STUGameInstance->Teams);
}

void ASTULobbyGameState::SpawnAllTeams_Multicast_Implementation(const TArray<FTeamInfo> &TeamsInfo)
{
    // Очистити живих персонажів (якщо вони — живі актори)
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

    // Оновлюємо локальний екземпляр GameInstance (якщо він є)
    if (STUGameInstance)
    {
        STUGameInstance->Teams = TeamsInfo;
    }

    // Отримати локальний гравецьський ID (безпечніше отримувати контролер)
    APlayerController *LocalPC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
    FString MyID;
    if (LocalPC && LocalPC->PlayerState)
    {
        MyID = LocalPC->PlayerState->GetUniqueId()->ToString();
    }

    // Знайти мою команду
    FPlayerInfo *MyPlayer = nullptr;
    if (!MyID.IsEmpty())
    {
        MyPlayer = FindPlayerByPlayerID(MyID);
    }

    int32 MyTeamID = MyPlayer ? MyPlayer->TeamID : INDEX_NONE;

    // Підготуємо масив команд для спавну: знайдемо мою команду та інші
    FTeamInfo *MyTeam = (MyTeamID != INDEX_NONE) ? FindTeamByTeamID(MyTeamID) : nullptr;

    // Спавнимо мою команду першою (логіка залежить від твоєї потреби)
    if (MyTeam)
    {
        SpawnTeam(*MyTeam);
    }

    // Потім інші команди
    for (FTeamInfo &Team : STUGameInstance->Teams)
    {
        if (&Team == MyTeam)
            continue;
        SpawnTeam(Team);
    }
}

// ----- Spawn/Respawn helpers -----

void ASTULobbyGameState::SpawnTeam(FTeamInfo &TeamInfo)
{
    for (FPlayerInfo &PlayerInfo : TeamInfo.PlayersInfos)
    {
        // Отримуємо позицію для спавну
        FVector Pos = GetNextPlayerStart();
        SpawnPlayer(&PlayerInfo, Pos);
    }
}

void ASTULobbyGameState::SpawnPlayer(FPlayerInfo *PlayerInfo, FVector Position)
{
    if (!PlayerInfo || !CharacterClass || !GetWorld())
        return;

    FTransform SpawnTransform(FRotator::ZeroRotator, Position);
    // SpawnActorDeferred повертає AActor*, але шаблон - тип актора
    ASTUBaseCharacter *Character = GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(
        CharacterClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (!Character)
        return;

    // Припустимо, що ASTUBaseCharacter має поле SpawnInfo типу FPlayerInfo або подібне
    Character->SpawnInfo = *PlayerInfo; // копіюємо дані
    UGameplayStatics::FinishSpawningActor(Character, SpawnTransform);

    // Зберігаємо у мапу Characters (як Ptr)
    Characters.Add(PlayerInfo->PlayerID, Character);
}

// ----- Position generator -----

FVector ASTULobbyGameState::GetNextPlayerStart()
{
    // Безпечні дефолти
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
        YStart -= YDifference * (RowMax - 1);
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
    // Знаходимо команду в локальному масиві Teams та замінюємо на нові дані
    FTeamInfo *Team = FindTeamByTeamID(NewTeam.TeamID);
    if (Team)
    {
        *Team = NewTeam; // копіюємо нові дані в існуючу структуру
        RespawnTeam(Team);
    }
}

void ASTULobbyGameState::OnPlayerChanged_Multicast_Implementation(FPlayerInfo PlayerChanged)
{
    FPlayerInfo *Player = FindPlayerByPlayerID(PlayerChanged.PlayerID);
    if (Player)
    {
        *Player = PlayerChanged; // копіюємо оновлені дані
        RespawnPlayer(Player);
    }
}

void ASTULobbyGameState::ChangeTeamName_Server_Implementation(const FString &TeamName, const FString &NewTeamName)
{
    FTeamInfo *Team = STUGameInstance->Teams.FindByPredicate([&](const FTeamInfo &T) { return T.TeamName == TeamName; });
    if (Team)
    {
        Team->TeamName = NewTeamName;
        OnTeamChanged_Multicast(*Team); // Multicast (передаємо по значенню)
    }
}

void ASTULobbyGameState::ChangeTeamColor_Server_Implementation(const FString &TeamName, const FLinearColor &Color)
{
    FTeamInfo *Team =
        STUGameInstance->Teams.FindByPredicate([&](const FTeamInfo &T) { return T.TeamName == TeamName; });
    if (Team)
    {
        Team->TeamColor = Color;
        OnTeamChanged_Multicast(*Team); // Multicast
    }
}

void ASTULobbyGameState::ChangeWeapons_Server_Implementation(TSubclassOf<ASTUBaseWeapon> WeaponToChoose,
                                                             const FString &PlayerID)
{
    FPlayerInfo *Player = FindPlayerByPlayerID(PlayerID);
    if (Player && WeaponToChoose)
    {
        if (WeaponsToChoose.Contains(WeaponToChoose))
        {
            Player->WeaponClass = WeaponToChoose;
            OnPlayerChanged_Multicast(*Player); // Multicast
        }
    }
}

// ----- Respawn helpers -----

void ASTULobbyGameState::RespawnTeam(FTeamInfo *TeamInfo)
{
    if (!TeamInfo)
        return;

    for (FPlayerInfo &PlayerInfo : TeamInfo->PlayersInfos)
    {
        RespawnPlayer(&PlayerInfo);
    }
}

void ASTULobbyGameState::RespawnPlayer(FPlayerInfo *PlayerInfo)
{
    if (!PlayerInfo)
        return;

    ASTUBaseCharacter **FoundPtr = Characters.Find(PlayerInfo->PlayerID);
    ASTUBaseCharacter *Character = FoundPtr ? *FoundPtr : nullptr;
    if (!Character)
        return;

    FVector PreviousPosition = Character->GetActorLocation();

    Character->Destroy();

    FVector StartPosition = PreviousPosition;
    FTransform NewTransform(StartRotation, StartPosition);
    ASTUBaseCharacter *PlayerCharacter = GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(
        BaseCharacter, NewTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (PlayerCharacter)
    {
        PlayerCharacter->SpawnInfo = DefaultPlayerInfo;
        UGameplayStatics::FinishSpawningActor(PlayerCharacter, NewTransform);
    }
}
// ----- Spawn lobby character -----

void ASTULobbyGameState::SpawnLobbyCharacter(APlayerController *Player)
{
    if (!Player)
        return;

    FVector StartPosition = GetNextPlayerStart();
    FTransform NewTransform(StartRotation, StartPosition);
    ASTUBaseCharacter *PlayerCharacter = GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(
        BaseCharacter, NewTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (PlayerCharacter)
    {
        PlayerCharacter->SpawnInfo = DefaultPlayerInfo;
        UGameplayStatics::FinishSpawningActor(PlayerCharacter, NewTransform);

        if (!LobbyCamera)
        {
            // реалізація SetCamera має бути у header/class
            SetCamera();
            if (!LobbyCamera)
                return;
        }

        if (Player && LobbyCamera)
        {
            Player->SetViewTarget(LobbyCamera);
        }
    }
}
void ASTULobbyGameState::SetCamera()
{
    TArray<AActor *> Cameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
    LobbyCamera = Cast<ACameraActor>(Cameras[0]);
    /*for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController *PC = It->Get();
        if (PC)
        {
            PC->SetViewTarget(LobbyCamera);
        }
    }*/
}
// ----- Find helpers -----

FTeamInfo *ASTULobbyGameState::FindTeamByTeamID(int32 TeamID)
{
    return STUGameInstance->Teams.FindByPredicate([TeamID](const FTeamInfo &Team) { return Team.TeamID == TeamID; });
}

FPlayerInfo *ASTULobbyGameState::FindPlayerByPlayerID(const FString &PlayerID)
{
    for (FTeamInfo &Team : STUGameInstance->Teams)
    {
        FPlayerInfo *Player =
            Team.PlayersInfos.FindByPredicate([&](const FPlayerInfo &P) { return P.PlayerID == PlayerID; });
        if (Player)
        {
            return Player;
        }
    }
    return nullptr;
}