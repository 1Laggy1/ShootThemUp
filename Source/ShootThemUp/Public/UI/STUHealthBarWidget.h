// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUHealthBarWidget.generated.h"

class UProgressBar;
UCLASS()
class SHOOTTHEMUP_API USTUHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    void SetHealthPercent(float Percent, bool ShowHealth);
  protected:
    virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
    UPROPERTY(meta = (BindWidget))
    UProgressBar *HealthProgressBar;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentVisibilityThreshold = 0.8;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentColorThreshold = 0.3f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor GoodColor = FLinearColor::White;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor BadColor = FLinearColor::Red;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float TimeToShowDefault = 5;
    float TimeToShow = 0;
};
