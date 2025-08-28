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
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox *LevelItemsBox;
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation *LoadAnim;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    virtual void NativeOnInitialized() override;
    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation *Animation) override;

  private:
    UPROPERTY()
    TArray<USTULevelItemWidget *> LevelItemWidgets;
    UFUNCTION()
    void OnStartGame();
    UFUNCTION()
    void OnQuitGame();

    void InitLevelItems();
    void OnLevelSelected(const FLevelData& Data);
    USTUGameInstance *GetSTUGameInstance() const;
};
