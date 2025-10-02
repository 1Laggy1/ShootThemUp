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
    float DamageDistanceStartFalloff = 10000.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float DamageDistanceEndFalloff = 20000.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float MinDamage = 5.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float TimeBetweenShots = 0.1f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float BulletSpreadDefault = 0.3f;
    float BulletSpread = BulletSpreadDefault;
    bool IsZoomed;
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";
    virtual void MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                               int32 InstigatorID) override;
    virtual void MakeShot() override;
    virtual void BeginPlay() override;
    virtual void MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                  int32 InstigatorID) override;
    virtual void MakeShotFX(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd) override;
    virtual FVector GetTraceData(FVector ViewLocation, FRotator ViewRotation) const override;
    //void MakeDamage(const FHitResult& HitResult);
    UFUNCTION(Server, Reliable)
    void MakeDamage_Server(const FHitResult &HitResult);
  private:
    FTimerHandle ShotTimerHandle;
    UPROPERTY()
    UNiagaraComponent *MuzzleFXComponent;
    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visible);
    void SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd);
};
