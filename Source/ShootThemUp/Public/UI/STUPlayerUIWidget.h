// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerUIWidget.generated.h"

class UProgressBar;
class UTextBlock;
UCLASS()
class SHOOTTHEMUP_API USTUPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    void SetHealthPercent(float Percent);
    void SetPlayerName(FString PlayerName);
    void SetPlayerColor(const FLinearColor& Color);
  protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar *HealthProgressBar;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerNameText;
    
};
