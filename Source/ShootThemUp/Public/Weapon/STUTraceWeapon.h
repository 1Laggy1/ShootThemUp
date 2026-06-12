// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STUTraceWeapon.generated.h"


class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
UCLASS()
class SHOOTTHEMUP_API ASTUTraceWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
  public:
    ASTUTraceWeapon();
    virtual void Zoom(bool Enable) override;
  protected:
    UPROPERTY(EditAnywhere, Category = "Stats")
    float DamageDistanceStartFalloff = 10000.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float DamageDistanceEndFalloff = 20000.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float MinDamage = 5.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float BulletSpreadDefault = 0.3f;
    float BulletSpread = BulletSpreadDefault;
    bool IsZoomed;
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent *WeaponFXComponent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem *TraceFX;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";
    FTimerHandle MuzzleFXTimerHandle;

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    float MuzzleFXDuration = 0.05f;

    UFUNCTION(Server, Reliable)
    void MakeDamage_Server(const FHitResult &HitResult);
    virtual FVector GetTraceData(FVector ViewLocation, FRotator ViewRotation) const override;
    virtual void MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                               int32 InstigatorID) override;
    virtual void MakeShot() override;
    virtual void BeginPlay() override;
    virtual void MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                  int32 InstigatorID) override;
    virtual void MakeShotFX(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd) override;

    UPROPERTY()
    UNiagaraComponent *MuzzleFXComponent;
    void ShootMuzzleFX();
    void SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd);
    void HideMuzzleFX();


};
