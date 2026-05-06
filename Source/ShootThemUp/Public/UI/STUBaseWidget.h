// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "STUBaseWidget.generated.h"

class USoundCue;
UCLASS()
class SHOOTTHEMUP_API USTUBaseWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    void Show();
  protected:
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation *ShowAnimation;
     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USoundCue* OpenSound;
};
