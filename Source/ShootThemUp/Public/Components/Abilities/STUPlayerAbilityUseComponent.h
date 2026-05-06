// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUPlayerAbilityUseComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAbilityCooldownFinished);

class ACharacter;
class USoundCue;
class UNiagaraSystem;
UCLASS(Blueprintable)
class SHOOTTHEMUP_API USTUPlayerAbilityUseComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUPlayerAbilityUseComponent();
    UFUNCTION(Server, Reliable)
    void StartUseAbility_Server();
    UFUNCTION(Server, Reliable)
    void StopUseAbility_Server();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;
    float GetAbilityCooldownRemaining()
    {
        return GetWorld()->GetTimerManager().GetTimerRemaining(AbilityCooldownTimerHandle);
    }
    float GetAbilityCooldown()
    {
        return Cooldown;
    }
    float GetAbilityCooldownRemainingPrecents()
    {
        return GetAbilityCooldownRemaining() / Cooldown;
    }
    FOnAbilityCooldownFinished OnCooldownFinished;
    virtual void CooldownFinished()
    {
        OnCooldownFinished.Broadcast();
    }
    int32 GetUseCount()
    {
        return UseCount;
    }
    UTexture2D *GetAbilityIcon() const
    {
        return AbilityIcon;
    }
    UFUNCTION(Client, Reliable)
    void StartTimer_Client(bool Start);
  protected:
    virtual bool StartUseAbility();
    virtual bool StopUseAbility();
    UFUNCTION(NetMulticast, Reliable)
    void AbilityCallback_Multicast();
    virtual void AbilityCallback();
    virtual bool Ability()
    {
        return true;
    }
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    USoundCue *UseSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    UNiagaraSystem *NiagaraEffect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    float Cooldown;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 UseCountDefault = 3;
    UPROPERTY(Replicated)
    int32 UseCount = UseCountDefault;
    FTimerHandle AbilityCooldownTimerHandle;

    virtual void BeginPlay() override;
    ACharacter *MyPlayer;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    UTexture2D *AbilityIcon;

  private:
};
