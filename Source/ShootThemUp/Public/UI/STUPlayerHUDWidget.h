// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "UI/STUBaseWidget.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

/**
 *
 */
class USTUWeaponComponent;
class USTUHealthActorComponent;
class UProgressBar;
class ASTUGameStateBase;
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
    GENERATED_BODY()
  public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetCurrentAmmo() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponUIData(FWeaponUIData &UIData) const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool isPlayerAlive() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool isPlayerSpectating() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetCurrentTime();
    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetRoundsInfo();
    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetKills();
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();
    virtual void NativeOnInitialized() override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentColorThreshold = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor GoodColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor BadColor = FLinearColor::Red;
  private:

    UPROPERTY(meta = (BindWidget))
    UProgressBar *HealthProgressBar;
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation *DamageAnimation;

    
    void OnHealthChanged(float Health);
    void OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser);
    ASTUGameStateBase *CurrentGamemodeState;
    void OnNewPawn(APawn *NewPawn);
    void UpdateHealthBar();
};
