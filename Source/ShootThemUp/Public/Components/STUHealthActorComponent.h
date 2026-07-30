// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUHealthActorComponent.generated.h"

class UPhysicalMaterial;
class UWidgetComponent;
class USTUHealthBarWidget;
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
     
    UFUNCTION()
    void Rep_HealthChanged()
    {
        //UpdateHealthWidget(nullptr, Health, false);
        OnHealthChanged.Broadcast(Health);
    }
    
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
    
      UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    TMap<UPhysicalMaterial *, float> DamageModifiers;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0"))
    float MaxHealth = 100.0f;
    virtual void BeginPlay() override;
    
  private:

    UPROPERTY(Replicated)//Using = Rep_HealthChanged)
    float Health = 0.0f;
    
    bool IsVaunded = false;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const;
    UFUNCTION()
    void OnTakePointDamage(AActor *DamagedActor, float Damage, class AController *InstigatedBy, FVector HitLocation,
                           class UPrimitiveComponent *FHitComponent, FName BoneName, FVector ShotFromDirection,
                           const class UDamageType *DamageType, AActor *DamageCauser);
    UFUNCTION(Server, Reliable)
    void ApplyDamageServer(AActor *DamagedActor, float Damage,
                           AActor* DamageCauser);
    UFUNCTION(NetMulticast, Reliable)
    void ApplyDamageMulticast(AActor *DamagedActor, float Damage, AActor *DamageCauser, float NewHealth);
    UFUNCTION(NetMulticast, Reliable)
    void DeathMulticast(int32 PlayerID);
    UFUNCTION(NetMulticast, Reliable)
    void HealthChangedMulticast(float NewHealth);
    UFUNCTION()
    void OnTakeRadialDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, FVector Origin,
                            const FHitResult &HitInfo, class AController *InstigatedBy, AActor *DamageCauser);
    void AutoHealHandle(float DeltaTime);
    void Killed(AController *KillerActor, AController *DiedActor);
    void ApplyDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy, const UDamageType *DamageType,
                     AActor *DamageCauser);
    float GetPointDamageModifier(AActor *DamagedActor, const FName &BoneName);
};
