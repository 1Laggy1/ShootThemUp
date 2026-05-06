// Shoot THem Up Game. All Rights Reserved.

#include "UI/STUItemUseWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Resources/STUUseableActor.h"

void USTUItemUseWidget::SetMyItem(ASTUUseableActor *Item)
{
    MyItem = Item;
    SetRenderOpacity(0.0f);
    if (ItemNameText)
    ItemNameText->SetText(FText::FromString(MyItem->GetItemName() + " (E)"));
}

void USTUItemUseWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!MyItem )
        return;

    if (MyItem->GetCooldownRemaining() > 0.0f)
    {
        if (!IsCooldownVisible)
        {
            SetCooldownVisible(true);
        }
        else
        {
            if (CooldownText)
            CooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), MyItem->GetCooldownRemaining())));
            if (MyItem->GetCooldown() > 0)
            {
                float TimeForProgressBar = MyItem->GetCooldownRemaining() / MyItem->GetCooldown();
                CooldownProgressBar->SetPercent(TimeForProgressBar);
            }
            
        }
    }
    else
    {
        if (IsCooldownVisible)
        {
            SetCooldownVisible(false);
        }
    }
}

void USTUItemUseWidget::SetCooldownVisible(bool IsActive)
{
    IsCooldownVisible = IsActive;
    ESlateVisibility Slate = IsActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
    CooldownText->SetVisibility(Slate);
    CooldownProgressBar->SetVisibility(Slate);
}
