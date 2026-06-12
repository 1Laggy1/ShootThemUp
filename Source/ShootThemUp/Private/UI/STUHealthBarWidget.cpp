// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUHealthBarWidget.h"
#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent, bool ShowHealth)
{
    if (!HealthProgressBar)
        return;
    if (ShowHealth)
    {
        TimeToShow = TimeToShowDefault;
        HealthProgressBar->SetVisibility(ESlateVisibility::Visible);
    }
    const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
    HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

    HealthProgressBar->SetPercent(Percent);
}

void USTUHealthBarWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    if (TimeToShow > 0.0f)
    {
        TimeToShow -= InDeltaTime;
        if (TimeToShow <= 0 && HealthProgressBar->IsVisible())
        {
            HealthProgressBar->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    
}
