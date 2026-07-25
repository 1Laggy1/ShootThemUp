// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "STULobbyHUD.generated.h"

class UUserWidget;
UCLASS()
class SHOOTTHEMUP_API ASTULobbyHUD : public AHUD
{
	GENERATED_BODY()
  public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LobbyWidgetClass;

  protected:
    virtual void BeginPlay() override;
    
};
