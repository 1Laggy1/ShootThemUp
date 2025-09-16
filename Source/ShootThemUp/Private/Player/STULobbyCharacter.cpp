// Shoot THem Up Game. All Rights Reserved.

#include "Player/STULobbyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Net/UnrealNetwork.h"
ASTULobbyCharacter::ASTULobbyCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
}

void ASTULobbyCharacter::BeginPlay()
{
    Super::BeginPlay();
    
}

ACameraActor *ASTULobbyCharacter::GetLobbyCamera() const
{
    return LobbyCamera;
}

ACameraActor *ASTULobbyCharacter::SpawnLobbyCamera()
{
    if (HasAuthority())
        LobbyCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), RelatedCameraPosition,
                                                           RelatedCameraRotation);
    LobbyCamera->SetReplicates(true);
    return LobbyCamera;
}
