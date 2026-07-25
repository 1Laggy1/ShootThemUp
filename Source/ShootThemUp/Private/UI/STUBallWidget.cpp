// Shoot THem Up Game. All Rights Reserved.
#include "UI/STUBallWidget.h"
#include "Components/Image.h"


void USTUBallWidget::ChangeColor(FLinearColor Color)
{
    if (Image)
    {
        Image->SetBrushTintColor(FSlateColor(Color));
    }
}
