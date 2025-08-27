// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTURespawnComponent();
	int32 GetRespawnCountDown()
	{
        return RespawnCountDown;
    }
    bool isRespawnInProgress() const;

  protected:
	virtual void BeginPlay() override;

public:	
	void Respawn(int32 RespawnTime);

private:
    FTimerHandle RespawnTimerHandle;
  int32 RespawnCountDown = 0;
    void RespawnTimerUpdate();

		
};
