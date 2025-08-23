// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUHealthActorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthActorComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUHealthActorComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    float GetHealth() const
    {
        return Health;
    }
    UFUNCTION(BlueprintCallable, Category = "Health")
    bool isDead() const
    {
        return FMath::IsNearlyZero(Health);
    }
    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return Health / MaxHealth; }
    float GetMaxHealth() const
    {
        return MaxHealth;
    }
    bool GetIsVaunded() const
    {
        return IsVaunded;
    }
    float TakeHeal(float amount);

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;
    FOnDamaged OnDamaged;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
              meta = (ClampMin = "0", EditCondition = "AutoHeal"))
    float HealDelay = 0.0f;
    float HealDelayCurrent = 0.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
              meta = (ClampMin = "0", EditCondition = "AutoHeal"))
    float HealAmount = 1.0f;

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0"))
    float MaxHealth = 100.0f;
    virtual void BeginPlay() override;

  private:
    float Health = 0.0f;
    bool IsVaunded = false;
    UFUNCTION()
    void OnTakeAnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                         class AController *InstigatedBy, AActor *DamageCauser);
    void AutoHealHandle(float DeltaTime);
};
