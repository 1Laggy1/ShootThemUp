// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerStatRowWidget.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget
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
    virtual void NativeOnInitialized() override;
  private:
    void OnMatchStateChanged(ESTUMatchState State);
    void UpdatePlayersStat();

    UFUNCTION()
    void OnResetLevel();
};
