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
class USlider;
class UEditableTextBox;
class USTUWeaponItemWidget;
class USTUAbilityItemWidget;
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
    UPROPERTY(meta = (BindWidget))
    UScrollBox *WeaponsItemsBox;
    UPROPERTY(meta = (BindWidget))
    UScrollBox *AbilityItemsBox;
    UPROPERTY(meta = (BindWidget))
    USlider *Slider_R;
    UPROPERTY(meta = (BindWidget))
    USlider *Slider_G;
    UPROPERTY(meta = (BindWidget))
    USlider *Slider_B;
    UPROPERTY(meta = (BindWidget))
    UButton *ChangeColorButton;
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox *TeamNameInputBox;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> WeaponWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> AbilityWidgetClass;
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation *LoadAnim;
    UFUNCTION()
    void OnChangeColorClicked();
    UFUNCTION()
    void HandleTeamNameCommitted(const FText & Text, ETextCommit::Type CommitMethod);
    virtual void NativeOnInitialized() override;
  private:
    UPROPERTY()
    TArray<USTULevelItemWidget *> LevelItemWidgets;
    UPROPERTY()
    TArray<USTUWeaponItemWidget *> WeaponItemWidgets;
    UPROPERTY()
    TArray<USTUAbilityItemWidget *> AbilityItemWidgets;
    UFUNCTION()
    void OnStartGame();
    UFUNCTION()
    void OnQuitGame();
    void InitLevelItems();
    void InitWeaponsItems();
    void InitAbilityItems();
    void OnLevelSelected(const FLevelData &Data);
    void OnWeaponSelected(const FWeaponItemData &Data);
    void OnAbilitySelected(const FAbilityItemData &Data);
    USTUGameInstance *GetSTUGameInstance() const;
};
