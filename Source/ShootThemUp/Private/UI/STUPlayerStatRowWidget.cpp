// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUPlayerStatRowWidget::SetRowInfo(FStatRowInfo &Info)
{
    SetText(PlayerNameTextBlock, Info.PlayerNameText);
    SetText(KillsTextBlock, Info.KillsText);
    SetText(DeathsTextBlock, Info.DeathsText);
    SetText(TeamTextBlock, Info.TeamText);
    SetPlayerIndicatorVisibility(Info.PlayerIndicatorVisibility);
    SetTeamColor(Info.TeamColor);

}

void USTUPlayerStatRowWidget::SetText(UTextBlock *TextBlock, const FString Text)
{
    if (!TextBlock)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetText failed: TextBlock is nullptr"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Setting text: %s"), *Text);
    TextBlock->SetText(FText::FromString(Text));
}

void USTUPlayerStatRowWidget::SetPlayerIndicatorVisibility(const bool VisibilityBool)
{
    if (!PlayerIndicatorImage)
        return;
    PlayerIndicatorImage->SetVisibility(VisibilityBool ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USTUPlayerStatRowWidget::SetTeamColor(FLinearColor &Color)
{
    if (!TeamColorImage)
        return;
    TeamColorImage->SetBrushTintColor(FSlateColor(Color));
}
