// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUItemWidget.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;
class UImage;
UCLASS()
class SHOOTTHEMUP_API USTUItemWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    

    
    void SetSelected(bool isSelected);

  protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor HoverColor = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);
    UPROPERTY(meta = (BindWidget))
    UButton *SelectButton;
    UPROPERTY(meta = (BindWidget))
    UTextBlock *NameTextBlock;
    UPROPERTY(meta = (BindWidget))
    UImage *Image = nullptr;
    UPROPERTY(meta = (BindWidget))
    UImage *FrameImage = nullptr;

    virtual void NativeOnInitialized() override;
    UFUNCTION()
    virtual void OnItemClicked();
    void SetItemData(const FItemData &Data);
  private:
    
    UFUNCTION()
    void OnItemHover();
    UFUNCTION()
    void OnItemUnHover();
};
