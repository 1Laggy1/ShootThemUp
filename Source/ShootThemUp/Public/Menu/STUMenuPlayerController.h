// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
  public:
  protected:
    virtual void BeginPlay() override;
  private:
    
};
