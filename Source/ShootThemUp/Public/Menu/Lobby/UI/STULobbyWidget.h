// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STULobbyWidget.generated.h"

/**
 * 
 */

class UButton;
class UHorizontalBox;
class USTULevelItemWidget;
class USTUGameInstance;
class UScrollBox;
UCLASS()
class SHOOTTHEMUP_API USTULobbyWidget : public USTUBaseWidget
{

	GENERATED_BODY()

  protected:
   
    UPROPERTY(meta = (BindWidget))
    UButton *StartGameButton;
    UPROPERTY(meta = (BindWidget))
    UButton *QuitGameButton;

    UPROPERTY(meta = (BindWidget))
    UScrollBox *LevelItemsBox;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation *LoadAnim;
    virtual void NativeOnInitialized() override;
  private:
    UPROPERTY()
    TArray<USTULevelItemWidget *> LevelItemWidgets;
    UFUNCTION()
    void OnStartGame();
    UFUNCTION()
    void OnQuitGame();
    void InitLevelItems();
    void OnLevelSelected(const FLevelData &Data);
    USTUGameInstance *GetSTUGameInstance() const;
};
