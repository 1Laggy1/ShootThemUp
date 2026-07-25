// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "Menu/UI/STUItemWidget.h"
#include "STULevelItemWidget.generated.h"


UCLASS()
class SHOOTTHEMUP_API USTULevelItemWidget : public USTUItemWidget
{
	GENERATED_BODY()
  public:
    FOnLevelSelectedSignature OnLevelSelected;
    void SetLevelData(const FLevelData &Data);
    FLevelData GetLevelData() const
    {
        return LevelData;
    }

  protected:
    virtual void OnItemClicked() override;
  private:
    FLevelData LevelData;
};
