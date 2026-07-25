// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/STUItemWidget.h"
#include "STUCoreTypes.h"
#include "STUAbilityItemWidget.generated.h"

    /**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAbilityItemWidget : public USTUItemWidget
{
	GENERATED_BODY()
  public:
    FOnAbilitySelectedSignature OnAbilitySelected;
    void SetAbilityData(const FAbilityItemData &Data);
    FAbilityItemData GetAbilityData() const
    {
        return AbilityData;
    }

  protected:
    virtual void OnItemClicked() override;

  private:
    FAbilityItemData AbilityData;
};
