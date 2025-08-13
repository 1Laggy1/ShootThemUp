// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponents;
UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

  public:
    ASTUBaseWeapon();
    virtual void Fire();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent *WeaponMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleFlashSocket";
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Damage = 10.0f;
    float TraceMaxDistance = 1500.0f;
    APlayerController *Controller;
    APlayerController *GetPlayerController() const;
    bool GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    void MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd);
    void MakeDamage(AActor *ActorToHit);
    virtual void BeginPlay() override;

    void MakeShot();

  private:
};
