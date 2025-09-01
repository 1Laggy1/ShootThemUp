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
    virtual void Zoom(bool Enable) override;
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
    virtual void MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation,
                                               int32 InstigatorID) override;
    virtual void MakeShot() override;
    virtual void BeginPlay() override;
    virtual void MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation,
                                                  int32 InstigatorID) override;
    virtual void MakeShotFX(FVector ViewLocation, FRotator ViewRotation) override;
    virtual FVector GetTraceData(FVector ViewLocation, FRotator ViewRotation) const override;
    void MakeDamage(const FHitResult& HitResult);
  private:
    FTimerHandle ShotTimerHandle;
    UPROPERTY()
    UNiagaraComponent *MuzzleFXComponent;
    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visible);
    void SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd);
};
