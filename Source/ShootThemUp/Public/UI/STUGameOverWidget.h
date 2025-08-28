// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerStatRowWidget.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    virtual bool Initialize() override;

  protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox *PlayerStatBox;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USTUPlayerStatRowWidget> PlayerStatRowWidgetClass;

  private:
    void OnMatchStateChanged(ESTUMatchState State);
    void UpdatePlayersStat();
};
