// Shoot THem Up Game. All Rights Reserved.

#include "Menu/UI/STULevelItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTULevelItemWidget::SetLevelData(const FLevelData &Data)
{
    LevelData = Data;
    SetItemData(Data);
    
}

void USTULevelItemWidget::OnItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}