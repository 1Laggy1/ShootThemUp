// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "UI/STUBaseWidget.h"
#include "STUMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;
class UWidgetAnimation;
UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
  protected:
    UPROPERTY(meta = (BindWidget))
    UButton *StartGameButton;
    UPROPERTY(meta = (BindWidget))
    UButton *QuitGameButton;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
    UWidgetAnimation *LoadAnim;

    

    virtual void NativeOnInitialized() override;
    //virtual void OnAnimationFinished_Implementation(const UWidgetAnimation *Animation) override;
    //virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
  private:
    
    UFUNCTION()
    void OnStartGame();
    UFUNCTION()
    void OnQuitGame();

   
    USTUGameInstance *GetSTUGameInstance() const;
};
