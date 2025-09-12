// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STULobbyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTULobbyCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
  protected:
    void BeginPlay() override;
};
