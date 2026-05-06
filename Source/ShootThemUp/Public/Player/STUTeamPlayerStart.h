// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "STUTeamPlayerStart.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUTeamPlayerStart : public APlayerStart
{
    GENERATED_BODY()
  public:
    int32 GetTeamID()
    {
        return TeamID;
    }

  private:
    UPROPERTY(EditAnywhere, Category = "Team")
    int32 TeamID;
};
