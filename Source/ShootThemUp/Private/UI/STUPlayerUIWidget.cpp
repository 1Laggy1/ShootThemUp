// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUPlayerUIWidget.h"
#include "Components/ProgressBar.h"

void USTUPlayerUIWidget::SetHealthPercent(float Percent)
{
    if (!HealthProgressBar)
        return;

    HealthProgressBar->SetPercent(Percent);
}
