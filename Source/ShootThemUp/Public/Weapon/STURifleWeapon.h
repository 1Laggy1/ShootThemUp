// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

/**
 * 
 */
class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
  public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

  protected:
    UPROPERTY(EditAnywhere, Category = "Stats")
    float TimeBetweenShots = 0.1f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float BulletSpread = 0.3f;
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    virtual void BeginPlay() override;
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd) const override;
    void MakeDamage(AActor *ActorToHit);
  private:
    FTimerHandle ShotTimerHandle;
    UPROPERTY()
    UNiagaraComponent *MuzzleFXComponent;
    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visible);
    void SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd);
};
