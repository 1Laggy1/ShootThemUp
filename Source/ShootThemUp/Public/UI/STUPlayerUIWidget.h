// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerUIWidget.generated.h"

class UProgressBar;
UCLASS()
class SHOOTTHEMUP_API USTUPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    void SetHealthPercent(float Percent);
  protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar *HealthProgressBar;
    
};
