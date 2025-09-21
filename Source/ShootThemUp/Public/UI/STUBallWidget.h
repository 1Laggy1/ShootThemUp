// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUBallWidget.generated.h"

/**
 * 
 */
class UImage;
UCLASS()
class SHOOTTHEMUP_API USTUBallWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    UPROPERTY(meta = (BindWidget))
    UImage *Image;
    void ChangeColor(FLinearColor Color);
};
