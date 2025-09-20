// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUUseableActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUUseableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASTUUseableActor();
  virtual void Use(FVector Location, FVector Rotation);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "STU")
    float CooldownTime = 5.0f;
    float CooldownRemaining = 0.0f;
    
    
	virtual void BeginPlay() override;

private:
    void CooldownTick();
	FTimerHandle CooldownTimerHandle;

};
