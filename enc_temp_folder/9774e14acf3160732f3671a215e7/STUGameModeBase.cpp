// Shoot THem Up Game. All Rights Reserved.

#include "STUGameModeBase.h"
#include "AIController.h"
#include "Algo/RandomShuffle.h"
#include "Camera/CameraActor.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"

#include "GameFramework/GameState.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "Player/STUPlayerState.h"
#include "Player/STUTeamPlayerStart.h"
#include "Resources/STUBall.h"
#include "Resources/STUBallSpawn.h"
#include "Resources/STUGoal.h"
#include "STUGameInstance.h"
#include "STUGameStateBase.h"
#include "STUUtils.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = nullptr;
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
    GameStateClass = ASTUGameStateBase::StaticClass();
    // SpectatorClass = ASpectatorPawn::StaticClass();
}

// FMatchStatistics ASTUGameModeBase::GetMatchStatistics()
//{
//     if (!GetWorld())
//         return FMatchStatistics();
//     FMatchStatistics MatchStatistics;
//     for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
//     {
//         const auto Controller = It->Get();
//         if (!Controller)
//             continue;
//         const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
//         if (!PlayerState)
//             continue;
//
//
//         MatchStatistics.Stats.Add(RowInfo);
//     }
//     return MatchStatistics;
// }
void ASTUGameModeBase::Goal(ASTUBall *Ball, ASTUGoal *Goal)
{

    /*FPlayerInfo *PlayerScored = STUUtils::FindPlayerByPlayerID(
        Ball->PreviousPlayerCharacter->Controller->PlayerState->GetUniqueId()->ToString(),
    STUGameStateBase->TeamsStats); if (PlayerScored)
    {*/
    if (GetWorldTimerManager().IsTimerActive(AfterGoalTimerHandle))
        return;

    FTeamInfo *TeamScored = STUUtils::FindTeamByTeamID(Goal->GetTeamID() == 1 ? 2 : 1, STUGameStateBase->TeamsStats);
    //FTeamInfo *TeamVictim = STUUtils::FindTeamByTeamID(Goal->GetTeamID(), STUGameStateBase->TeamsStats);
    /*if (TeamScored->TeamID != TeamVictim->TeamID)
    {*/
    if (TeamScored)
        TeamScored->Score++;
    //}
    //}
    STUGameStateBase->AfterGoalCountDown = GameData.AfterGoalTime;
    GetWorld()->GetTimerManager().PauseTimer(GameRoundTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(AfterGoalTimerHandle, this, &ASTUGameModeBase::AfterGoalTimerUpdate, 1.0f,
                                           true);
}
void ASTUGameModeBase::StartPlay()
{

    Super::StartPlay();
    if (!GetWorld() || !GetWorld()->HasBegunPlay())
    {
        UE_LOG(LogTemp, Warning, TEXT("World not ready in StartPlay"));
        return;
    }
    auto GState = GetWorld()->GetGameState();
    STUGameStateBase = Cast<ASTUGameStateBase>(GState);
    STUGameInstance = GetGameInstance<USTUGameInstance>();
    if (STUGameInstance && STUGameStateBase->TeamsStats.IsEmpty())
    {
        STUGameStateBase->SetGameData(GameData, STUGameInstance->Teams);
        PlayersNum = 0;
        for (const FTeamInfo &Team : STUGameInstance->Teams)
        {
            PlayersNum += Team.PlayersInfos.Num();
            UE_LOG(LogTemp, Warning, TEXT("Team has %d players"), Team.PlayersInfos.Num());
        }
    }
    GetTeamsStarts();
    FindAllBallSpawns();
    GetWorld()->GetTimerManager().SetTimer(WaitingForPlayersTimerHandle, this, &ASTUGameModeBase::WaitingForPlayers,
                                           1.0f, true);
    // STUGameStateBase->CurrentRound = 0;
    //  SpawnBots();
    //  CreateTeamsInfo();
}

void ASTUGameModeBase::GetTeamsStarts()
{
    if (!GetWorld())
        return;
    TArray<AActor *> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTUTeamPlayerStart::StaticClass(), PlayerStarts);
    for (FTeamInfo &Team : STUGameStateBase->TeamsStats)
    {
        Team.TeamStarts.Empty();
        for (AActor *Start : PlayerStarts)
        {
            const auto TeamStart = Cast<ASTUTeamPlayerStart>(Start);
            if (TeamStart && TeamStart->GetTeamID() == Team.TeamID)
            {
                Team.TeamStarts.Add(TeamStart);
            }
        }
        UE_LOG(LogSTUGameModeBase, Display, TEXT("Team %d has %d starts"), Team.TeamID, Team.TeamStarts.Num());
        Algo::RandomShuffle(Team.TeamStarts);
    }
}

AActor *ASTUGameModeBase::GetRandomTeamStart(int32 TeamID)
{
    if (!GetWorld() || STUGameStateBase->TeamsStats.IsEmpty())
        return nullptr;
    FTeamInfo *Team = STUUtils::FindTeamByTeamID(TeamID, STUGameStateBase->TeamsStats);
    if (!Team || Team->TeamStarts.IsEmpty())
        return nullptr;
    int32 RandIndex = FMath::RandRange(0, Team->TeamStarts.Num() - 1);
    return Team->TeamStarts[RandIndex];
}

void ASTUGameModeBase::SpawnBall(int32 TeamID)
{
    if (BallInstance)
    {
        BallInstance->Destroy();
    }
    ASTUBallSpawn *BallSpawn = nullptr;
    if (TeamID == 0)
    {
        BallSpawn = GetRandomBallSpawn();
        BallSpawn->StartSpawning(GameData.BallSpawnTime);
        if (!BallSpawn)
            return;
        
        return;
    }

    FTeamInfo *Team = STUUtils::FindTeamByTeamID(TeamID, STUGameStateBase->TeamsStats);
    if (!Team || !Team->BallSpawnPoint)
        return;
    /*BallInstance = GetWorld()->SpawnActor<ASTUBall>(BallClass, Team->BallSpawnPoint->GetActorLocation(),
                                                    Team->BallSpawnPoint->GetActorRotation());*/
    //Team->BallSpawnPoint->StartSpawning(GameData.BallSpawnTime);
}

ASTUBallSpawn *ASTUGameModeBase::GetRandomBallSpawn()
{
    if (!GetWorld())
        return nullptr;
    if (DefaultBallSpawns.IsEmpty())
        return nullptr;
    int32 RandIndex = FMath::RandRange(0, DefaultBallSpawns.Num() - 1);
    return DefaultBallSpawns[RandIndex];
}

void ASTUGameModeBase::FindAllBallSpawns()
{
    if (!GetWorld())
        return;
    DefaultBallSpawns.Empty();
    TArray<AActor *> BallSpawns;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTUBallSpawn::StaticClass(), BallSpawns);
    for (AActor *Spawn : BallSpawns)
    {
        const auto BallSpawn = Cast<ASTUBallSpawn>(Spawn);
        if (BallSpawn)
        {
            if (BallSpawn->GetTeamID() == 0)
            {
                DefaultBallSpawns.Add(BallSpawn);
                continue;
            }

            FTeamInfo *Team = STUUtils::FindTeamByTeamID(BallSpawn->GetTeamID(), STUGameStateBase->TeamsStats);
            if (Team)
            {
                Team->BallSpawnPoint = BallSpawn;
            }
        }
    }
}

void ASTUGameModeBase::SetPlayerStatsToAllPlayers()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller)
            continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            continue;
        FString PlayerID = Controller->PlayerState->GetUniqueId().IsValid()
                               ? Controller->PlayerState->GetUniqueId()->ToString()
                               : TEXT("UnknownID");
        PlayerState->SetPlayerStats(PlayerID);
    }
}

void ASTUGameModeBase::SetMovement(bool Active)
{
    TArray<AActor *> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTUBaseCharacter::StaticClass(), FoundActors);
    for (AActor* ActorFound : FoundActors)
    {
        const auto Character = Cast<ASTUBaseCharacter>(ActorFound);
        if (Character)
            Character->MovementEnabled = Active;
    }
}



void ASTUGameModeBase::WaitingForPlayers()
{
    if (PlayersReady == PlayersNum && !BeforeStart)
    {
        BeforeStart = true;
        STUGameStateBase->WaitingTimeNow = GameData.BeforeStartTime;
    }
    if (STUGameStateBase->WaitingTimeNow <= 0)
    {
        SetPlayerStatsToAllPlayers();
        GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f,
                                               true);
        StartRound();

        return;
    }
    STUGameStateBase->WaitingTimeNow -= 1.0f;
    STUGameStateBase->OnRep_TimerChanged();
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Waiting for players: %f %d/%d"), STUGameStateBase->WaitingTimeNow,
           PlayersReady, PlayersNum);
}

UClass *ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController *InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    else
    {
        return Super::GetDefaultPawnClassForController_Implementation(InController);
    }
}

void ASTUGameModeBase::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld())
        return;
    /*for (int32 i = 0; i < GameData.PlayersNum - 1; i++)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(STUAIController);
    }*/
}

void ASTUGameModeBase::StartRound()
{
    ChangeState(ESTUMatchState::InProgress);
    // STUGameStateBase->CurrentRound++;
    ResetPlayers();
    // STUGameStateBase->RoundCountDown = GameData.RoundTime;
    SpawnBall(0);
    GetWorldTimerManager().PauseTimer(GameRoundTimerHandle);
    STUGameStateBase->BetweenGoalsCountDown = GameData.BetweenGoalsRespawnTime;
    SetMovement(false);
    GetWorld()->GetTimerManager().SetTimer(BetweenGoalsTimerHandle, this, &ASTUGameModeBase::BetweenGoalsTimerUpdate,
                                           1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    // UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound,
    // GameData.RoundsNum);
    STUGameStateBase->GameCountDown--;
    STUGameStateBase->OnRep_TimerChanged();
    if (STUGameStateBase->GameCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        /*if (STUGameStateBase->CurrentRound + 1 <= GameData.RoundsNum)
        {

            StartRound();
        }
        else
        {*/
        GameOver();
        //}
    }
}

void ASTUGameModeBase::BetweenGoalsTimerUpdate()
{
    STUGameStateBase->BetweenGoalsCountDown--;
    STUGameStateBase->OnRep_TimerChanged();
    if (STUGameStateBase->BetweenGoalsCountDown <= 0)
    {
        GetWorldTimerManager().ClearTimer(BetweenGoalsTimerHandle);
        SetMovement(true);
        GetWorldTimerManager().UnPauseTimer(GameRoundTimerHandle);
        /*if (STUGameStateBase->CurrentRound + 1 <= GameData.RoundsNum)
        {

            StartRound();
        }
        else
        {*/
        // GameOver();
        // }
    }
}

void ASTUGameModeBase::AfterGoalTimerUpdate()
{


    if (STUGameStateBase->AfterGoalCountDown-- <= 0)
    {
        GetWorldTimerManager().ClearTimer(AfterGoalTimerHandle);
        StartRound();
        /*if (STUGameStateBase->CurrentRound + 1 <= GameData.RoundsNum)
        {

            StartRound();
        }
        else
        {*/
        // GameOver();
        // }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld())
        return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController *Controller)
{
    if (Controller->GetPawn())
    {
        Controller->GetPawn()->Destroy();
    }
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (PlayerState)
    {
        PlayerState->SetSTUPlayerState(STUPlayerStateEnum::Gaming);
    }
    /*UE_LOG(LogGameMode, Error, TEXT("----------------------PlayerState is %s, %s"),
           *FindPlayerStart(Controller)->GetFullName(), *FindPlayerStart(Controller)->GetActorLocation().ToString());

    RestartPlayer(Controller);*/
    /*AActor* Spawn = GetRandomSpawnPoint(GetWorld());
    APawn *NewDefaultPawn = SpawnDefaultPawnFor(Controller, Spawn);*/
    AActor *Spawn = GetRandomTeamStart(
        STUUtils::FindPlayerByPlayerID(PlayerState->GetUniqueId()->ToString(), STUGameInstance->Teams)->TeamID);
    FVector SpawnLocation = Spawn ? Spawn->GetActorLocation() : FVector::Zero();
    FRotator SpawnRotation = Spawn ? Spawn->GetActorRotation() : FRotator::ZeroRotator;
    FTransform SpawnTransform(SpawnRotation, SpawnLocation);

    ASTUBaseCharacter *NewCharacter = GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(
        DefaultCharacterClass, SpawnTransform, Controller, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    NewCharacter->SetReplicates(true);
    if (NewCharacter)
    {

        FString PlayerID = Controller->PlayerState->GetUniqueId().IsValid()
                               ? Controller->PlayerState->GetUniqueId()->ToString()
                               : TEXT("UnknownID");
        FPlayerInfo *PlayerInfo = STUUtils::FindPlayerByPlayerID(PlayerID, STUGameInstance->Teams);

        NewCharacter->PlayerColor = PlayerInfo->Color;
        NewCharacter->PlayerName = Controller->PlayerState->GetPlayerName();
        NewCharacter->PlayerID = PlayerID;

        UGameplayStatics::FinishSpawningActor(NewCharacter, SpawnTransform);
        // NewCharacter->OnRep_PlayerID();
    }
    // auto STUPlayerController = Cast<ASTUPlayerController>(Controller);
    // if (!STUPlayerController)
    //     return;
    // STUPlayerController->Possess(NewCharacter);
    // STUPlayerController->ControlledPawn = NewCharacter;
    // STUPlayerController->OnRep_Possesed();
    /*FString PlayerID = Controller->PlayerState->GetUniqueId().IsValid()
                           ? Controller->PlayerState->GetUniqueId()->ToString()
                           : TEXT("UnknownID");*/
    // STUGameStateBase->InitPlayer_Multicast(PlayerID, NewCharacter);
    //  Controller->Possess(NewCharacter);
}

void ASTUGameModeBase::SetPlayerColor(AActor *Player, FLinearColor TeamColor)
{
    if (!Player)
        return;
    // STUGameStateBase->SetPlayerColorMulticast(Player, TeamColor);
}

AActor *ASTUGameModeBase::GetRandomSpawnPoint(UWorld *World)
{
    TArray<AActor *> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.Num() == 0)
        return nullptr;
    return PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];
}
// void ASTUGameModeBase::SetPlayerInfo(APlayerController *Controller)
//{
//     if (!Controller)
//         return;
//     const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
//     if (!PlayerState)
//         return;
//
//     PlayerState->SetTeamID(TeamIDNow);
//
//     PlayerState->SetTeamColor(
//         DetermineColorByTeamID(TeamIDNow)); /*if (!Controller->IsPlayerController()) { FString Bot; Bot = "Bot " +
//                                             FString::FromInt(BotNum); PlayerState->SetPlayerName(Bot); BotNum++; }*/
//     PlayerState->SetSTUPlayerState(STUPlayerStateEnum::Gaming);
//     SetPlayerColor(Controller, DetermineColorByTeamID(TeamIDNow));
//     TeamIDNow = TeamIDNow == 1 ? 2 : 1;
//     if (!STUGameStateBase)
//         return;
//
//     /*FPlayerStats newPlayerStats;
//     newPlayerStats.PlayerName = PlayerState->GetPlayerName();
//     newPlayerStats.Kills = PlayerState->GetKillsNum();
//     newPlayerStats.Deaths = PlayerState->GetDeathsNum();
//     newPlayerStats.TeamID = PlayerState->GetTeamID();*/
//     //STUGameStateBase->MatchStatistics.Stats.Add(STUUtils::GetPlayerStatsFromPlayerState(PlayerState));
// }

void ASTUGameModeBase::ChangeState(ESTUMatchState NewState)
{
    if (STUGameStateBase)
    {
        STUGameStateBase->SetMatchState(NewState);
        STUGameStateBase->OnMatchStateChanged.Broadcast(NewState);
    }
}

void ASTUGameModeBase::RespawnAsSpectator(AController *Controller, FVector DeathLocation, FRotator DeathRotation)
{
    if (!Controller)
        return;
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Controller;
    ASpectatorPawn *Spectator =
        GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, DeathLocation, DeathRotation, SpawnParams);
    const auto PlayerController = Cast<ASTUPlayerController>(Controller);
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);

    if (Spectator && PlayerController && PlayerState)
    {
        Controller->UnPossess();
        Controller->Possess(Spectator);
        PlayerState->SetSTUPlayerState(STUPlayerStateEnum::Spectating);
        PlayerController->SetViewTargetWithBlend(Spectator, 0.0f);
        PlayerController->SetInputMode(FInputModeGameOnly());
        PlayerController->bShowMouseCursor = false;
        PlayerController->StartSpectatingMulticast(Spectator);
    }
}

// FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
//{
//     if (TeamID - 1 < GameData.TeamColors.Num())
//     {
//         return GameData.TeamColors[TeamID - 1];
//     }
//     UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID,
//            *GameData.DefaultTeamColor.ToString());
//     return GameData.DefaultTeamColor;
// }

void ASTUGameModeBase::LogPlayerInfo()
{
    if (!GetWorld())
        return;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller)
            continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            return;
        PlayerState->LogInfo();
    }
}
void ASTUGameModeBase::StartRespawn(AController *DiedActor)
{
    if (!DiedActor)
        return;
    const auto RespawnComponent = DiedActor->FindComponentByClass<USTURespawnComponent>();
    if (!RespawnComponent)
        return;
    const auto PlayerController = Cast<ASTUPlayerController>(DiedActor);
    if (PlayerController)
    {

        RespawnAsSpectator(DiedActor, PlayerController->GetPreviousCameraPosition(),
                           PlayerController->GetPreviousCameraRotation());
    }

    RespawnComponent->Respawn(GameData.RespawnTime);

    // FVector PreviousPosition = DiedActor->GetPawn()->GetActorLocation();
    // DiedActor->ChangeState(NAME_Spectating);
    // DiedActor->GetPawn()->SetActorLocation(PreviousPosition);
}

void ASTUGameModeBase::GameOver()
{
    if (!GetWorld())
        return;

    UE_LOG(LogSTUGameModeBase, Display, TEXT("====================== GAME OVER ======================"))
    LogPlayerInfo();

    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
    ChangeState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::RespawnRequest(AController *Controller)
{
    ResetOnePlayer(Controller);
}

void ASTUGameModeBase::Killed(AController *KillerActor, AController *DiedActor)
{

    const auto VictimPlayerState = DiedActor ? Cast<ASTUPlayerState>(DiedActor->PlayerState) : nullptr;
    const auto KillerPlayerState = KillerActor ? Cast<ASTUPlayerState>(KillerActor->PlayerState) : nullptr;

    if (KillerPlayerState && VictimPlayerState)
    {
        FPlayerInfo *KillerInfo = FindPlayerByPlayerID(KillerPlayerState->GetUniqueId().IsValid()
                                                           ? KillerPlayerState->GetUniqueId()->ToString()
                                                           : TEXT("UnknownID"));
        FPlayerInfo *VictimInfo = FindPlayerByPlayerID(VictimPlayerState->GetUniqueId().IsValid()
                                                           ? VictimPlayerState->GetUniqueId()->ToString()
                                                           : TEXT("UnknownID"));
        if (KillerInfo && VictimInfo)
        {
            if (KillerInfo->TeamID == VictimInfo->TeamID)
            {
                KillerInfo->Kills--;
            }
            else
            {
                KillerInfo->Kills++;
                VictimInfo->Deaths++;
            }
        }
    }
    StartRespawn(DiedActor);
}
FPlayerInfo *ASTUGameModeBase::FindPlayerByPlayerID(const FString &PlayerID)
{
    for (FTeamInfo &Team : STUGameStateBase->TeamsStats)
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
void ASTUGameModeBase::PostSeamlessTravel()
{
    Super::PostSeamlessTravel();

    UE_LOG(LogSTUGameModeBase, Display, TEXT("PostSeamlessTravel called"));
}
bool ASTUGameModeBase::SetPause(APlayerController *PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        ChangeState(ESTUMatchState::Pause);
    }
    return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
    const auto PauseSet = Super::ClearPause();
    if (PauseSet)
    {
        ChangeState(ESTUMatchState::InProgress);
    }
    return PauseSet;
}

void ASTUGameModeBase::PlayerConnected(APlayerController *PC)
{
    if (PC && !PlayersReadyIDs.Contains(PC->PlayerState->GetUniqueId()->ToString()))
    {
        PlayersReady++;
        PlayersReadyIDs.Add(PC->PlayerState->GetUniqueId()->ToString());

        /*auto GState = GetWorld()->GetGameState();
        STUGameStateBase = Cast<ASTUGameStateBase>(GState);
        USTUGameInstance *STUGameInstance = GetGameInstance<USTUGameInstance>();
        if (STUGameInstance && STUGameStateBase->TeamsStats.IsEmpty())
        {
            STUGameStateBase->SetGameData(GameData, STUGameInstance->Teams);
        }
        UE_LOG(LogSTUGameModeBase, Display, TEXT("DefaultPawnClass=%s"), *GetNameSafe(DefaultCharacterClass));
        ResetOnePlayer(PC);*/
    }
}
