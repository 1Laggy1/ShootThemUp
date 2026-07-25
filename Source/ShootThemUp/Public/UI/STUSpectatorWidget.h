// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUSpectatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUSpectatorWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetRespawnTime(int32 &CountDownTime) const;
};
