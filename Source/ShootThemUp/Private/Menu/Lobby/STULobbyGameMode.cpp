// Shoot THem Up Game. All Rights Reserved.


#include "Menu/Lobby/STULobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Menu/Lobby/STULobbyGameState.h"
#include "Player/STUBaseCharacter.h"
#include "GameFramework/PlayerStart.h"
ASTULobbyGameMode::ASTULobbyGameMode()
{
    DefaultPawnClass = nullptr;
    SpectatorClass = nullptr;

    GameStateClass = ASTULobbyGameState::StaticClass();
    bUseSeamlessTravel = true;
}

void ASTULobbyGameMode::StartPlay()
{
    Super::StartPlay();
    //BeginFindPlayerStarts();
    XNow = XStart;
    YNow = YStart;
    SetCamera();
}



void ASTULobbyGameMode::BeginFindPlayerStarts()
{
    TArray<AActor *> FoundStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundStarts);

    for (AActor *Actor : FoundStarts)
    {
        if (Actor)
        {
            const auto Start = Cast<ASTULobbyPlayerStart>(Actor);
            if (Start)
            {
                PlayerStarts.Add(Start);
            }
        }
    }
}

FVector ASTULobbyGameMode::GetNextPlayerStart()
{
    FVector NextStartPosition = FVector::ZeroVector;

    if (FirstSpawnPosition)
    {
        XNow = XStart;
        YNow = YStart;
        FirstSpawnPosition = false;
        return FVector(XNow, YNow, Height);
    }
    
    if (Row == RowMax)
    {
        XNow = XStart;
        YStart -= YDifference * (RowMax-2);
        YNow = YStart;
        Row = 1;
        return FVector(XNow, YNow, Height);
    }
    else
    {
        Row++;
        XNow += XDifference;
        YNow += YDifference;
    }

    

    return FVector(XNow, YNow, Height);
}

void ASTULobbyGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);
    SpawnLobbyCharacter(NewPlayer);
}

void ASTULobbyGameMode::SpawnLobbyCharacter(APlayerController *Player)
{
    FVector StartPosition = GetNextPlayerStart();
    FTransform NewTransform(StartRotation, StartPosition);
    const auto PlayerCharacter =
        GetWorld()->SpawnActorDeferred<ASTUBaseCharacter>(BaseCharacter, NewTransform, NULL);
    if (PlayerCharacter)
    {
        PlayerCharacter->SpawnInfo = DefaultSpawnInfo;
        UGameplayStatics::FinishSpawningActor(PlayerCharacter, NewTransform);
        //Player->Possess(PlayerCharacter);
        //PlayerCharacter->DispatchBeginPlay();
        if (!LobbyCamera)
        {
            SetCamera();
            if (!LobbyCamera)
                return;
        }
        if (GameState && LobbyCamera)
        {
            Player->SetViewTarget(LobbyCamera);
        }
    }
}

void ASTULobbyGameMode::SetCamera()
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
void ASTULobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController *NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);
    
}
