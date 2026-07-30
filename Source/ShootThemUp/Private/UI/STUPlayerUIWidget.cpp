// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUPlayerUIWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USTUPlayerUIWidget::SetHealthPercent(float Percent)
{
    if (!HealthProgressBar)
        return;

    HealthProgressBar->SetPercent(Percent);
}

void USTUPlayerUIWidget::SetPlayerName(FString PlayerName)
{
    if (PlayerNameText)
    {
        PlayerNameText->SetText(FText::FromString(PlayerName));
    }
}

void USTUPlayerUIWidget::SetPlayerColor(const FLinearColor& Color)
{
    if (PlayerNameText)
    {
        PlayerNameText->SetColorAndOpacity(FSlateColor(Color));
        float Luminance = (Color.R * 0.299f) + (Color.G * 0.587f) + (Color.B * 0.114f);
        FLinearColor OutlineColor = (Luminance > 0.5f) ? FLinearColor::Black : FLinearColor::White;
        FSlateFontInfo FontInfo = PlayerNameText->GetFont();
        FontInfo.OutlineSettings.OutlineColor = OutlineColor;
        PlayerNameText->SetFont(FontInfo);
    }
    if (HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(Color);
    }
}
