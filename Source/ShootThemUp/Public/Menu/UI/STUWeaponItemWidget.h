// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/STUItemWidget.h"
#include "STUCoreTypes.h"
#include "STUWeaponItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUWeaponItemWidget : public USTUItemWidget
{
	GENERATED_BODY()
  public:
    FOnWeaponSelectedSignature OnWeaponSelected;
    void SetWeaponData(const FWeaponItemData &Data);
    FWeaponItemData GetWeaponData() const
    {
        return WeaponData;
    }

  protected:
    virtual void OnItemClicked() override;

  private:
    FWeaponItemData WeaponData;
};
