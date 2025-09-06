// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

/**
 * 
 */
class ASTUProjectile;

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
  public:
    virtual void StartFire() override;

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUProjectile> ProjectileClass;
    virtual void MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                  int32 InstigatorID) override;
    virtual void MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                  int32 InstigatorID) override;
    UFUNCTION(Server, Reliable)
    void SpawnProjectileServer(FVector Direction);
    int ShotsFires = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int ShotsToFire = 3;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    float ShotInterval = 0.2f;
    FTimerHandle BurstTimerHandle;
    void MakeBurstShot();
    void FakeShot(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd);
};
