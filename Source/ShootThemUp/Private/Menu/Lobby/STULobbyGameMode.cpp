// Shoot THem Up Game. All Rights Reserved.


#include "Menu/Lobby/STULobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Menu/Lobby/STULobbyGameState.h"
#include "Player/STUBaseCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Player/STUPlayerController.h"

#include "GameFramework/PlayerState.h"
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

    
}



//void ASTULobbyGameMode::BeginFindPlayerStarts()
//{
//    TArray<AActor *> FoundStarts;
//    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundStarts);
//
//    for (AActor *Actor : FoundStarts)
//    {
//        if (Actor)
//        {
//            const auto Start = Cast<ASTULobbyPlayerStart>(Actor);
//            if (Start)
//            {
//                PlayerStarts.Add(Start);
//            }
//        }
//    }
//}


void ASTULobbyGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);
    STUOnPostLogin.Broadcast(NewPlayer);
    /*NewPlayer->PlayerState->GetUniqueId();
    SpawnLobbyCharacter(NewPlayer);*/
}




void ASTULobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController *NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);
    
}
