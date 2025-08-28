// Shoot THem Up Game. All Rights Reserved.

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();
    SpawnBots();
    CreateTeamsInfo();
    CurrentRound = 1;
    StartRound();

     SetMatchState(ESTUMatchState::InProgress);
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




void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld())
        return;
    for (int32 i = 0; i < GameData.PlayersNum - 1; i++)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(STUAIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);
    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
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
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld())
        return;
    int32 TeamID = 1;
    int32 BotNum = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller)
            continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            return;

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        if (!Controller->IsPlayerController())
        {
            FString Bot;
            Bot = "Bot " + FString::FromInt(BotNum);
            PlayerState->SetPlayerName(Bot);
                BotNum++;
        }
        SetPlayerColor(Controller);
        TeamID = TeamID == 1 ? 2 : 1;
        
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

void ASTUGameModeBase::SetPlayerColor(AController *Controller)
{
    if (!Controller)
        return;

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if (!Character)
        return;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState)
        return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
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
void ASTUGameModeBase::StartRespawn(AController *Controller)
{
    if (!Controller)
        return;
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if (!RespawnComponent)
        return;

    RespawnComponent->Respawn(GameData.RespawnTime);
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
    SetMatchState(ESTUMatchState::GameOver);
}



void ASTUGameModeBase::RespawnRequest(AController *Controller)
{
    ResetOnePlayer(Controller);
}

void ASTUGameModeBase::Killed(AController *KillerController, AController *VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = KillerController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }
    StartRespawn(VictimController);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
    if (MatchState == State)
        return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}

bool ASTUGameModeBase::SetPause(APlayerController *PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        SetMatchState(ESTUMatchState::Pause);
    }
    return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
    const auto PauseSet = Super::ClearPause();
    if (PauseSet)
    {
        SetMatchState(ESTUMatchState::InProgress);
    }
    return PauseSet;
}
