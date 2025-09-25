// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUItemUseWidget.generated.h"

class UProgressBar;
class UTextBlock;
class ASTUUseableActor;
UCLASS()
class SHOOTTHEMUP_API USTUItemUseWidget : public UUserWidget
{
	GENERATED_BODY()
  public: 
      void SetMyItem(ASTUUseableActor *Item);
  protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock *ItemNameText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar *CooldownProgressBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock *CooldownText;
    virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
    virtual void SetCooldownVisible(bool IsActive);
  private:

    ASTUUseableActor *MyItem;
    bool IsCooldownVisible;
};
