// Shoot THem Up Game. All Rights Reserved.


#include "Menu/UI/STUAbilityItemWidget.h"

void USTUAbilityItemWidget::SetAbilityData(const FAbilityItemData &Data)
{
    AbilityData = Data;
    SetItemData(Data);
}

void USTUAbilityItemWidget::OnItemClicked()
{
    OnAbilitySelected.Broadcast(AbilityData);
}