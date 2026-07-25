// Shoot THem Up Game. All Rights Reserved.


#include "Menu/UI/STUItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTUItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (SelectButton)
    {
        SelectButton->OnClicked.AddDynamic(this, &USTUItemWidget::OnItemClicked);
        SelectButton->OnHovered.AddDynamic(this, &USTUItemWidget::OnItemHover);
        SelectButton->OnUnhovered.AddDynamic(this, &USTUItemWidget::OnItemUnHover);
    }
}



void USTUItemWidget::OnItemClicked()
{
    
}
void USTUItemWidget::OnItemHover()
{
    Image->SetColorAndOpacity(HoverColor);
}
void USTUItemWidget::OnItemUnHover()
{
    Image->SetColorAndOpacity(FLinearColor::White);
}
void USTUItemWidget::SetItemData(const FItemData &Data)
{
    if (NameTextBlock)
    {
        NameTextBlock->SetText(FText::FromName(Data.DisplayName));
    }
    if (Image)
    {
        Image->SetBrushFromTexture(Data.Thumb);
    }
}
void USTUItemWidget::SetSelected(bool isSelected)
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(isSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}