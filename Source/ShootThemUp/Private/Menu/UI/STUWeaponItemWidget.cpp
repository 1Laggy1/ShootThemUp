// Shoot THem Up Game. All Rights Reserved.


#include "Menu/UI/STUWeaponItemWidget.h"

 void USTUWeaponItemWidget::SetWeaponData(const FWeaponItemData &Data)
{
    WeaponData = Data;
    SetItemData(Data);
}

void USTUWeaponItemWidget::OnItemClicked()
{
    OnWeaponSelected.Broadcast(WeaponData);
}