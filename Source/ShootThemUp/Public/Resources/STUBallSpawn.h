// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBallSpawn.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBallSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBallSpawn();
  int32 GetTeamID() const
  {
      return TeamID;
  }

protected:
	
private:
  UPROPERTY(EditAnywhere, Category = "Team")
	int32 TeamID = 0;
};
