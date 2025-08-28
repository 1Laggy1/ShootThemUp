// Shoot THem Up Game. All Rights Reserved.

#include "Menu/UI/STULevelItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
    }
}

void USTULevelItemWidget::SetLevelData(const FLevelData &Data)
{
    LevelData = Data;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }
    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(Data.LevelThumb);
    }

}

void USTULevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}
void USTULevelItemWidget::SetSelected(bool isSelected)
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(isSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}