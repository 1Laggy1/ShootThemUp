// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "STULobbyPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTULobbyPlayerStart : public APlayerStart
{
	GENERATED_BODY()
  public:
    bool IsUsed;
};
