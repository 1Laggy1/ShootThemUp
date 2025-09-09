// Shoot THem Up Game. All Rights Reserved.


#include "Menu/Lobby/STULobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void ASTULobbyPlayerController::SetCamera()
{
    TArray<AActor *> Cameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
    auto LobbyCamera = Cast<ACameraActor>(Cameras[0]);
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController *PC = It->Get();
        if (PC)
        {
            PC->SetViewTarget(LobbyCamera);
        }
    }
}

void ASTULobbyPlayerController::OnPossess(APawn *aPawn)
{
    Super::OnPossess(aPawn);
    SetCamera();
}
