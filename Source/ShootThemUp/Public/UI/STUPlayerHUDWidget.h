// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

/**
 * 
 */
class USTUWeaponComponent;
class USTUHealthActorComponent;
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
  public:
	UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetCurrentAmmo() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool isPlayerAlive() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool isPlayerSpectating() const;
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();
    virtual bool Initialize() override;
  private:
    void OnDamaged();
	
};
