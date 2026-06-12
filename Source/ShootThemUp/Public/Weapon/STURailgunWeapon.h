// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUSingleShotTraceWeapon.h"
#include "STURailgunWeapon.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTURailgunWeapon : public ASTUSingleShotTraceWeapon
{
	GENERATED_BODY()
	
  public:
    virtual void StartFire() override;
    virtual void StopFire() override;
    virtual void Tick(float DeltaTime) override;

    

  protected:
    ASTURailgunWeapon();
	UPROPERTY(EditAnywhere, Category = "Stats")
    float TimeToCharge = 1.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VFX")
    UNiagaraComponent *ChargeNiagaraComponent;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UNiagaraSystem *ChargeNiagaraSystem;
    UPROPERTY(EditAnywhere, Category = "VFX")
    float ChargeScaleMultiplier = 10.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UAudioComponent *ChargeAudioComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundBase *ChargeSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundBase *DischargeSound;

  private:
	bool bIsCharging;
    float Charge;
    float ChargePercent;
    float ChargePercentPrevious;
    void StartCharge();
    UFUNCTION(Server, Reliable)
    void StartCharge_Server();
    UFUNCTION(NetMulticast, Reliable)
    void StartCharge_Multicast();
    void Discharge();
    UFUNCTION(Server, Reliable)
    void StopCharge_Server();
    UFUNCTION(NetMulticast, Reliable)
    void StopCharge_Multicast();
    void ChargeComplete();
    FTimerHandle ChargeTimerHandle;

};
