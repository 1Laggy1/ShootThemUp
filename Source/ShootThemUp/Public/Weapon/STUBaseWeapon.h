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
    virtual void StartFire();
    virtual void StopFire();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent *WeaponMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleFlashSocket";
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Damage = 10.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float TraceMaxDistance = 5000.0f;
    
    APlayerController *Controller;
    APlayerController *GetPlayerController() const;
    bool GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;
    
    void MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd);
    
    virtual void BeginPlay() override;

    virtual void MakeShot();
    virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd) const;

  

};
