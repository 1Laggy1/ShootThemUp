// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STULobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTULobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
  public:
    FPlayerSpawnInfo SpawnInfo;
    void SetCamera();

  protected:
    virtual void OnPossess(APawn *aPawn) override;
};
