// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"






class USkeletalMeshComponents;
UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

  public:
    ASTUBaseWeapon();
    FOnClipSignature OnClipEmpty;
    virtual void StartFire();
    virtual void StopFire();
    void ChangeClip();
    bool CanReload() const;
  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent *WeaponMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleFlashSocket";
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Damage = 10.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float TraceMaxDistance = 5000.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    FAmmoData DefaultAmmo{15,10,false};
    APlayerController *Controller;
    APlayerController *GetPlayerController() const;
    bool GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;
    
    void MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd);
    
    virtual void BeginPlay() override;

    virtual void MakeShot();
    virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd) const;

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool isClipEmpty() const;
    
    void LogAmmo();

    private:
    FAmmoData CurrentAmmo;

};
