// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"
class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUProjectile();

	void SetShotDirection(const FVector &Direction)
    {
        ShotDirection = Direction;
    }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
  USphereComponent *CollisionComponent;
    
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
  UProjectileMovementComponent *MovementComponent;
    UPROPERTY(EditDefaultsOnly, Category = "Stat")
  float DamageRadius = 200.0f;

  UPROPERTY(EditDefaultsOnly, Category = "Stat")
  float DamageAmount = 50.0f;
  UPROPERTY(EditDefaultsOnly, Category = "Stat")
  bool DoFullDamage = false;
  UPROPERTY(EditDefaultsOnly, Category = "Stat")
  float LifeSeconds = 5.0f;
  UPROPERTY(VisibleAnywhere, Category = "VFX")
  USTUWeaponFXComponent *WeaponFXComponent;
	virtual void BeginPlay() override;

private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                         UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    AController *GetController() const;
};
