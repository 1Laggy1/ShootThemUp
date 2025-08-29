// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STULevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
UCLASS()
class SHOOTTHEMUP_API USTULevelItemWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    FOnLevelSelectedSignature OnLevelSelected;

    void SetLevelData(const FLevelData &Data);
    FLevelData GetLevelData() const
    {
        return LevelData;
    }
    void SetSelected(bool isSelected);
  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor HoverColor = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);
    UPROPERTY(meta = (BindWidget))
    UButton *LevelSelectButton;
    UPROPERTY(meta = (BindWidget))
    UTextBlock *LevelNameTextBlock;
    UPROPERTY(meta = (BindWidget))
    UImage *LevelImage = nullptr; 
    UPROPERTY(meta = (BindWidget))
    UImage *FrameImage = nullptr;

    virtual void NativeOnInitialized() override;
    private:
    FLevelData LevelData;
      UFUNCTION()
    void OnLevelItemClicked();
      UFUNCTION()
      void OnLevelItemHover();
      UFUNCTION()
    void OnLevelItemUnHover();
};
