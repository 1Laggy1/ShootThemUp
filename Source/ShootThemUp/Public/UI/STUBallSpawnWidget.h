// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUBallSpawnWidget.generated.h"

/**
 * 
 */
class UProgressBar;
class UTextBlock;
class ASTUBallSpawn;
UCLASS()
class SHOOTTHEMUP_API USTUBallSpawnWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
    ASTUBallSpawn *BallSpawnInstance;
    void SetVisible(bool isActive);
  protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar *SpawnProgressBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock *SpawnText;
    virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

  private:
    

    
    void SetProgress(float Percent);

    
    void SetText(const FText &NewText);
};
