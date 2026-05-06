// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STULobbyCharacter.generated.h"

/**
 *
 */
class UCameraComponent;
class ACameraActor;
UCLASS()
class SHOOTTHEMUP_API ASTULobbyCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()
  public:
    ASTULobbyCharacter(const FObjectInitializer &ObjInit = FObjectInitializer::Get());

    ACameraActor *GetLobbyCamera() const;
    ACameraActor *SpawnLobbyCamera();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    ACameraActor *LobbyCamera;
    void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraSpawn")
    FVector RelatedCameraPosition;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraSpawn")
    FRotator RelatedCameraRotation;
};
