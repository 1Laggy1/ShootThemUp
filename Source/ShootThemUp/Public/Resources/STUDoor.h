// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Resources/STUUseableActor.h"
#include "STUDoor.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUDoor : public ASTUUseableActor
{
	GENERATED_BODY()
  public:
    ASTUDoor();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    FVector ClosedLocation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    FVector OpenLocation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    float Speed;
    virtual void Tick(float DeltaTime) override;
    virtual bool Use(FVector Location, FVector Rotation, AController *Instigator) override;

  protected:
  private:
    bool IsClosed = true;
    bool AnimDone = true;
};
