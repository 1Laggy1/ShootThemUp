// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;
UCLASS()
class SHOOTTHEMUP_API USTUPlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    void SetRowInfo(FStatRowInfo &Info);
    void SetText(UTextBlock* TextBlock, const FString Text);
    void SetPlayerIndicatorVisibility(const bool VisibilityBool);
  protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock *PlayerNameTextBlock;
    UPROPERTY(meta = (BindWidget))
    UTextBlock *KillsTextBlock;
    UPROPERTY(meta = (BindWidget))
    UTextBlock *DeathsTextBlock;
    UPROPERTY(meta = (BindWidget))
    UTextBlock *TeamTextBlock;
    UPROPERTY(meta = (BindWidget))
    UImage *PlayerIndicatorImage;
};
