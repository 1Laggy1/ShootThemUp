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
        LevelSelectButton->OnHovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemHover);
        LevelSelectButton->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemUnHover);

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
void USTULevelItemWidget::OnLevelItemHover()
{
    LevelImage->SetColorAndOpacity(HoverColor);
}
void USTULevelItemWidget::OnLevelItemUnHover()
{
    LevelImage->SetColorAndOpacity(FLinearColor::White);
}
void USTULevelItemWidget::SetSelected(bool isSelected)
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(isSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}