// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Resources/STUInteractable.h"
#include "STUJumppad.generated.h"

/**
 * 
 */
class ACharacter;

UCLASS()
class SHOOTTHEMUP_API ASTUJumppad : public ASTUInteractable
{
	GENERATED_BODY()
	public:
	
	protected:
      virtual void Interact(AActor *OtherActor) override;
	private:
      UPROPERTY(EditAnywhere, Category = "Pickup")
      float JumpHeight = 1.0f;
      float JumpMultiplier = 500.0f;
      void Jump(ACharacter *character);

	
};
