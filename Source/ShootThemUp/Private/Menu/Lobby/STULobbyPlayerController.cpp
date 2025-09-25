// Shoot THem Up Game. All Rights Reserved.

#include "Menu/Lobby/STULobbyPlayerController.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void ASTULobbyPlayerController::SetCamera_Implementation(FVector Location, FRotator Rotation)
{

    /*FActorSpawnParameters SpawnParams;
   SpawnParams.Owner = this;
   ACameraActor *TempCamera = GetWorld()->SpawnActor<ACameraActor>(Location, Rotation, SpawnParams);
   SetViewTarget(TempCamera);

   TArray<AActor *> Cameras;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
   for (AActor* Camera : Cameras)
   {
       if (Camera != TempCamera)
       Camera->Destroy();
   }*/
}

void ASTULobbyPlayerController::OnPossess(APawn *aPawn)
{
    Super::OnPossess(aPawn);
}

void ASTULobbyPlayerController::BeginPlay()
{
    bShowMouseCursor = true;
    TArray<AActor *> Cameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
    if (!Cameras.IsEmpty() || !Cameras[0])
        return;

    SetViewTarget(Cameras[0]);
}
