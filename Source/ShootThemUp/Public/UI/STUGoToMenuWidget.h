// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGoToMenuWidget.generated.h"

class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    virtual void NativeOnInitialized() override;
    UFUNCTION()
    void OnMainMenu();
  protected:
    UPROPERTY(meta = (BindWidget))
    UButton *GoToMenuButton;

  private:
    
};
