// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPauseWidget.generated.h"

/**
 * 
 */
class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    virtual bool Initialize() override;

  protected:
    UPROPERTY(meta = (BindWidget))
    UButton *ClearPauseButton;

  private:
    UFUNCTION()
    void OnClearPause();
};
