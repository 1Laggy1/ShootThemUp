// Shoot THem Up Game. All Rights Reserved.

#include "STUGameModeBase.h"
#include "AIController.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "Player/STUPlayerState.h"
#include "STUGameStateBase.h"
#include "STUUtils.h"
#include "UI/STUGameHUD.h"
#include "Algo/RandomShuffle.h"
#include "STUGameInstance.h"
#include "OnlineSubsystem.h"
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

//FMatchStatistics ASTUGameModeBase::GetMatchStatistics()
//{
//    if (!GetWorld())
//        return FMatchStatistics();
//    FMatchStatistics MatchStatistics;
//    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
//    {
//        const auto Controller = It->Get();
//        if (!Controller)
//            continue;
//        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
//        if (!PlayerState)
//            continue;
//        
//        
//        MatchStatistics.Stats.Add(RowInfo);
//    }
//    return MatchStatistics;
//}

void ASTUGameModeBase::StartPlay()
{

    Super::StartPlay();
    STUGameStateBase = GetGameState<ASTUGameStateBase>();
    if (STUGameStateBase)
    {
        STUGameStateBase->SetGameData(GameData);
    }
    // SpawnBots();
    // CreateTeamsInfo();
    STUGameStateBase->CurrentRound = 1;
    StartRound();
    ChangeState(ESTUMatchState::InProgress);

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
    UE_LOG(LogSTUGameModeBase, Display, TEXT("PostLogin: NewPlayer=%s"), *GetNameSafe(NewPlayer));
    UE_LOG(LogSTUGameModeBase, Display, TEXT("DefaultPawnClass=%s"), *GetNameSafe(DefaultCharacterClass));
    // RestartPlayer(NewPlayer);
    SetPlayerInfo(NewPlayer);
    ResetOnePlayer(NewPlayer);
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
    STUGameStateBase->RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    // UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound,
    // GameData.RoundsNum);
    if (STUGameStateBase->RoundCountDown-- == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        if (STUGameStateBase->CurrentRound + 1 <= GameData.RoundsNum)
        {
            STUGameStateBase->CurrentRound++;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
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
    

    AActor *Spawn = GetRandomSpawnPoint(GetWorld());
    FTransform SpawnTransform(Spawn->GetActorRotation(), Spawn->GetActorLocation());

    ASTUBaseCharacter *NewCharacter = GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(
        DefaultCharacterClass, SpawnTransform, Controller, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    if (NewCharacter)
    {

        FString PlayerID = Controller->PlayerState->GetUniqueId().IsValid()
                               ? Controller->PlayerState->GetUniqueId()->ToString()
                               : TEXT("UnknownID");
        FPlayerInfo *PlayerInfo = STUUtils::FindPlayerByPlayerID(PlayerID, Cast<USTUGameInstance>(GetGameInstance()));
        if (PlayerInfo)
        {
            NewCharacter->SpawnInfo = *PlayerInfo;
        }

        UGameplayStatics::FinishSpawningActor(NewCharacter, SpawnTransform);
    }
    Controller->Possess(NewCharacter);
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

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld())
        return;
    int32 TeamID = 1;
    int32 BotNum = 1;
    for (TActorIterator<ASTUBaseCharacter> It(GetWorld()); It; ++It)
    {
        const auto Character = *It;
        if (!Character)
            continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Character->GetPlayerState<ASTUPlayerState>());
        if (!PlayerState)
            return;

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));

        /*if (!Controller->IsPlayerController())
        {
            FString Bot;
            Bot = "Bot " + FString::FromInt(BotNum);
            PlayerState->SetPlayerName(Bot);
                BotNum++;
        }*/
        SetPlayerColor(Character, DetermineColorByTeamID(TeamID));
        TeamID = TeamID == 1 ? 2 : 1;
    }
}
void ASTUGameModeBase::SetPlayerInfo(APlayerController *Controller)
{
    if (!Controller)
        return;
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState)
        return;
    
    PlayerState->SetTeamID(TeamIDNow);
    
    PlayerState->SetTeamColor(
        DetermineColorByTeamID(TeamIDNow)); /*if (!Controller->IsPlayerController()) { FString Bot; Bot = "Bot " +
                                            FString::FromInt(BotNum); PlayerState->SetPlayerName(Bot); BotNum++; }*/
    PlayerState->SetSTUPlayerState(STUPlayerStateEnum::Gaming);
    SetPlayerColor(Controller, DetermineColorByTeamID(TeamIDNow));
    TeamIDNow = TeamIDNow == 1 ? 2 : 1;
    if (!STUGameStateBase)
        return;

    /*FPlayerStats newPlayerStats;
    newPlayerStats.PlayerName = PlayerState->GetPlayerName();
    newPlayerStats.Kills = PlayerState->GetKillsNum();
    newPlayerStats.Deaths = PlayerState->GetDeathsNum();
    newPlayerStats.TeamID = PlayerState->GetTeamID();*/
    //STUGameStateBase->MatchStatistics.Stats.Add(STUUtils::GetPlayerStatsFromPlayerState(PlayerState));
}

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

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID,
           *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

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
        if (KillerPlayerState->GetTeamID() == VictimPlayerState->GetTeamID())
        {
            KillerPlayerState->RemoveKill();
        }
        else
            KillerPlayerState->AddKill();

        if (STUGameStateBase)
        {
            //FPlayerStats *KillerStats = STUGameStateBase->MatchStatistics.Stats.FindByPredicate(
                //[&](const FPlayerStats &E) { return E.PlayerName == KillerPlayerState->GetPlayerName(); });
            /*if (KillerStats)
            {
                KillerStats->Kills = KillerPlayerState->GetKillsNum();
            }*/
        }
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
        if (STUGameStateBase)
        {
            //FPlayerStats *VictimStats = STUGameStateBase->MatchStatistics.Stats.FindByPredicate(
                //[&](const FPlayerStats &E) { return E.PlayerName == VictimPlayerState->GetPlayerName(); });
            /*if (VictimStats)
            {
                VictimStats->Deaths = VictimPlayerState->GetDeathsNum();
            }*/
        }
    }
    StartRespawn(DiedActor);
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
