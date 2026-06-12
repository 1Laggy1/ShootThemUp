// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerStatRowWidget.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
class UButton;
class UTextBlock;
UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public USTUBaseWidget
{
	GENERATED_BODY()
  public:
    

  protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox *PlayerStatBox;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USTUPlayerStatRowWidget> PlayerStatRowWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UButton *ResetLevelButton;
    UPROPERTY(meta = (BindWidget))
    UTextBlock *TeamWinText;
    virtual void NativeOnInitialized() override;
  private:
    void OnMatchStateChanged(ESTUMatchState State);
    void UpdatePlayersStat();

    UFUNCTION()
    void OnResetLevel();
};
