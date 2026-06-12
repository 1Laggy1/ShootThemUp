// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUPauseWidget.generated.h"
/**
 * 
 */
class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()
  public:
    virtual void NativeOnInitialized() override;

  protected:
    UPROPERTY(meta = (BindWidget))
    UButton *ClearPauseButton;
    
    

  private:
    UFUNCTION()
    void OnClearPause();
};
