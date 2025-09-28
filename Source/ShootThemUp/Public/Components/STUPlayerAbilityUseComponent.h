// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUPlayerAbilityUseComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAbilityCooldownFinished);

class ACharacter;
class SoundCue;
class UNiagaraSystem;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
    FOnAbilityCooldownFinished OnCooldownFinished;
  protected:
    virtual bool StartUseAbility();
    virtual bool StopUseAbility();
    UFUNCTION(NetMulticast, Reliable)
    void AbilityCallback_Multicast();
    virtual void AbilityCallback();


    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    USoundCue *UseSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    UNiagaraSystem *NiagaraEffect;
    FTimerHandle AbilityCooldownTimerHandle;

    virtual void BeginPlay() override;
  private:
    ACharacter *MyPlayer;

    void CooldownFinished()
    {
        OnCooldownFinished.Broadcast();
    }
};
