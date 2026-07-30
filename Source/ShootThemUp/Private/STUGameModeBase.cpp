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
#include "Weapon/STUBaseWeapon.h"
#include "STUUtils.h"
#include "UI/STUGameHUD.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = nullptr;
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
    static ConstructorHelpers::FClassFinder<ASTUGameStateBase> GameStateBPClass(TEXT("/Game/BP_STUGameStateBase.BP_STUGameStateBase_C"));
    
    if (GameStateBPClass.Succeeded())
    {
        GameStateClass = GameStateBPClass.Class;
    }
    else
    {
        GameStateClass = ASTUGameStateBase::StaticClass();
    }
}
void ASTUGameModeBase::Goal(ASTUBall *Ball, ASTUGoal *Goal)
{

    if (GetWorldTimerManager().IsTimerActive(AfterGoalTimerHandle))
        return;

    FTeamInfo *TeamScored = STUUtils::FindTeamByTeamID(Goal->GetTeamID() == 1 ? 2 : 1, STUGameStateBase->GetTeams());
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

    if (IntroCameras.IsEmpty())
        {
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("IntroCamera"), IntroCameras);
        }
        if (IntroCameras.Num() > 0)
        {
            int32 RandomIndex = FMath::RandHelper(IntroCameras.Num());  
            AActor* RandomCamera = IntroCameras[RandomIndex];
            STUGameStateBase->IntroCamera = RandomCamera;
        }

    TransferGameData();
    
    GetTeamsStarts();
    FindAllBallSpawns();
    if (isDebug)
    {
        WaitingForPlayers();
    }
    else 
    {
        GetWorld()->GetTimerManager().SetTimer(WaitingForPlayersTimerHandle, this, &ASTUGameModeBase::WaitingForPlayers,
                                           1.0f, true);
    }
    
}

void ASTUGameModeBase::TransferGameData()
{
    if (STUGameStateBase && !STUGameStateBase->GetTeams().IsEmpty())
    {
        return;
    }
    STUGameStateBase = GetGameState<ASTUGameStateBase>();
    STUGameInstance = GetGameInstance<USTUGameInstance>();
    if (STUGameStateBase && STUGameStateBase->GetTeams().IsEmpty())
    {
        if (STUGameInstance && !STUGameInstance->GetTeams().IsEmpty())
        {
            STUGameStateBase->SetGameData(GameData, STUGameInstance->GetTeams());
            PlayersNum = 0;
            for (const FTeamInfo &Team : STUGameInstance->GetTeams())
            {
                PlayersNum += Team.PlayersInfos.Num();
                UE_LOG(LogTemp, Warning, TEXT("Team has %d players"), Team.PlayersInfos.Num());
            
            }
        }
        else {
            StartDebug();
        }

        
        
    }
    
    

}



void ASTUGameModeBase::StartDebug()
{
    isDebug = true;
    TArray<FTeamInfo> TIA;
    
    FTeamInfo TI;
    TI.TeamID = 1;

    
    FTeamInfo TI2;
    TI2.TeamID = 2;

    

    PlayersNum++;
    FPlayerInfo playerInfo;
    playerInfo.WeaponClass = DebugWeaponClass;
    playerInfo.AbilityClass = DebugAbilityClass;
    playerInfo.PlayerName = TEXT("DebugPlayer");
    playerInfo.TeamID = 1;
    if (GetWorld())
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            playerInfo.ThisPlayerController = PC;

            if (PC->PlayerState && PC->PlayerState->GetUniqueId().IsValid())
            {
                playerInfo.PlayerID = PC->PlayerState->GetUniqueId()->ToString();
            }
            else
            {

                playerInfo.PlayerID = TEXT("DebugUniqueID");
            }
        }
    }

    TI.PlayersInfos.Add(playerInfo);
    TIA.Add(TI);
    TIA.Add(TI2);
    STUGameStateBase->SetGameData(GameData, TIA);
}   

void ASTUGameModeBase::SpawnDummy(int32 count)
{
    for (int i = 1; i <= count; i++)
    {
        PlayersNum++;
        FPlayerInfo playerInfo;
        playerInfo.WeaponClass = DebugWeaponClass;
        playerInfo.AbilityClass = DebugAbilityClass;
        playerInfo.PlayerName = TEXT("DebugPlayer");
        playerInfo.TeamID = PlayersNum % 2 == 0 ? 2 : 1;
        playerInfo.Color = playerInfo.TeamID == 1 ? FLinearColor::White : FLinearColor::Black;
        
        FString RandomStr = FGuid::NewGuid().ToString();
        playerInfo.PlayerID = RandomStr;
        FTeamInfo *Team = STUUtils::FindTeamByTeamID(playerInfo.TeamID, STUGameStateBase->GetTeams());
        Team->PlayersInfos.Add(playerInfo);
        ResetOnePlayer(playerInfo);
    }
    
}

void ASTUGameModeBase::GetTeamsStarts()
{
    if (!GetWorld())
        return;
    TArray<AActor *> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTUTeamPlayerStart::StaticClass(), PlayerStarts);
    for (FTeamInfo &Team : STUGameStateBase->GetTeams())
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
    if (!GetWorld() || STUGameStateBase->GetTeams().IsEmpty())
        return nullptr;
    FTeamInfo *Team = STUUtils::FindTeamByTeamID(TeamID, STUGameStateBase->GetTeams());
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

    FTeamInfo *Team = STUUtils::FindTeamByTeamID(TeamID, STUGameStateBase->GetTeams());
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

            FTeamInfo *Team = STUUtils::FindTeamByTeamID(BallSpawn->GetTeamID(), STUGameStateBase->GetTeams());
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
        if (!isDebug)
        {
            STUGameStateBase->WaitingTimeNow = GameData.BeforeStartTime;
        }
        else {
            STUGameStateBase->WaitingTimeNow = 0;
        }
        
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

void ASTUGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    RegisterPlayer(NewPlayer);
}

void ASTUGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
    Super::HandleSeamlessTravelPlayer(C);
    RegisterPlayer(C);
}

void ASTUGameModeBase::RegisterPlayer(AController* Controller)
{
    
    APlayerController* PC = Cast<APlayerController>(Controller);
    if (!PC || !PC->PlayerState)
    {
        return;
    }

    
        FString PlayerUniqueID = PC->PlayerState->GetUniqueId()->ToString();

        if (!PlayersReadyIDs.Contains(PlayerUniqueID))
        {
            PlayersReady++;
            PlayersReadyIDs.Add(PlayerUniqueID);
            
            TransferGameData(); 

            if (STUGameStateBase)
            {
                FPlayerInfo* PlayerConnected = STUUtils::FindPlayerByPlayerID(PlayerUniqueID, STUGameStateBase->GetTeams());
                if (PlayerConnected)
                {
                    
                    PlayerConnected->ThisPlayerController = PC;
                }
                
            }
        }
    
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
    STUGameStateBase->BetweenGoalsCountDown = isDebug ? 0 : GameData.BetweenGoalsRespawnTime;
    SetMovement(false);
    GetWorld()->GetTimerManager().SetTimer(BetweenGoalsTimerHandle, this, &ASTUGameModeBase::BetweenGoalsTimerUpdate,
                                           1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
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
        
        
    }
}

void ASTUGameModeBase::AfterGoalTimerUpdate()
{


    if (STUGameStateBase->AfterGoalCountDown-- <= 0)
    {
        GetWorldTimerManager().ClearTimer(AfterGoalTimerHandle);
        StartRound();
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld())
        return;

    if (STUGameStateBase && !STUGameStateBase->GetTeams().IsEmpty())
    {
        for (FTeamInfo& Team : STUGameStateBase->GetTeams())
        {
            for (FPlayerInfo& Player : Team.PlayersInfos)
            {
                ResetOnePlayer(Player);
            }

        }
        
    }
}

void ASTUGameModeBase::ResetOnePlayer(FPlayerInfo& PlayerInfo)
{

    if (IsValid(PlayerInfo.ThisPlayerController) && IsValid(PlayerInfo.ThisPlayerController->GetPawn()))
    {
        PlayerInfo.ThisPlayerController->GetPawn()->Destroy();
        const auto PlayerState = Cast<ASTUPlayerState>(PlayerInfo.ThisPlayerController->PlayerState);
        if (PlayerState)
        {
            PlayerState->SetSTUPlayerState(STUPlayerStateEnum::Gaming);
        }
    }
    AActor *Spawn = GetRandomTeamStart(PlayerInfo.TeamID);
    FVector SpawnLocation = Spawn ? Spawn->GetActorLocation() : FVector::Zero();
    FRotator SpawnRotation = Spawn ? Spawn->GetActorRotation() : FRotator::ZeroRotator;
    FTransform SpawnTransform(SpawnRotation, SpawnLocation);
    ASTUBaseCharacter *NewCharacter = GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(
        DefaultCharacterClass, SpawnTransform, PlayerInfo.ThisPlayerController, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    
    if (NewCharacter)
    {

        NewCharacter->SetReplicates(true);
        NewCharacter->SpawnInfo = PlayerInfo;
        NewCharacter->PlayerColor = PlayerInfo.Color;
        NewCharacter->PlayerName = PlayerInfo.PlayerName;
        NewCharacter->PlayerID = PlayerInfo.PlayerID;
        NewCharacter->TeamID = PlayerInfo.TeamID;
        if (!PlayerInfo.AbilityClass.Get())
        {
            NewCharacter->AbilityClass = DefaultAbilityClass.Get();
        }
        else
        {
            NewCharacter->AbilityClass = PlayerInfo.AbilityClass.Get();
        }
        UGameplayStatics::FinishSpawningActor(NewCharacter, SpawnTransform);
        
    }
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

void ASTUGameModeBase::RespawnRequest(FString PlayerID)
{
    FPlayerInfo* PlayerInfo = STUUtils::FindPlayerByPlayerID(PlayerID, STUGameStateBase->GetTeams());
    if (PlayerInfo)
    {
        ResetOnePlayer(*PlayerInfo);
    }
    else
    {
        UE_LOG(LogSTUGameModeBase, Error, TEXT("RespawnRequest: FindPlayerByPlayerID returned nullptr!"));
    }
}
void ASTUGameModeBase::Killed(AController *KillerActor, AController *DiedActor)
{

    const auto VictimPlayerState = DiedActor ? Cast<ASTUPlayerState>(DiedActor->PlayerState) : nullptr;
    const auto KillerPlayerState = KillerActor ? Cast<ASTUPlayerState>(KillerActor->PlayerState) : nullptr;

    if (VictimPlayerState)
    {
        
        
        FPlayerInfo *VictimInfo = STUUtils::FindPlayerByPlayerID(VictimPlayerState->GetUniqueId().IsValid()
                                                           ? VictimPlayerState->GetUniqueId()->ToString()
                                                           : TEXT("UnknownID"), STUGameStateBase->GetTeams());
        if (VictimInfo)
            VictimInfo->Deaths++;
        
        if (KillerPlayerState)
        {
            FPlayerInfo *KillerInfo = STUUtils::FindPlayerByPlayerID(KillerPlayerState->GetUniqueId().IsValid()
                                                               ? KillerPlayerState->GetUniqueId()->ToString()
                                                               : TEXT("UnknownID"), STUGameStateBase->GetTeams());
            if (KillerInfo && VictimInfo)
            {
                if (KillerInfo->PlayerID == VictimInfo->PlayerID)
                {
                    KillerInfo->Deaths++;
                }
                else if (KillerInfo->TeamID == VictimInfo->TeamID)
                {
                    KillerInfo->Kills--;
                }
                else
                {
                    KillerInfo->Kills++;
                }
            }
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
