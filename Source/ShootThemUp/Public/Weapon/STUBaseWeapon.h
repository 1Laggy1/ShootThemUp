// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"


class USkeletalMeshComponents;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;
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
    FWeaponUIData GetUIData() const
    {
        return UIData;
    }
    FAmmoData GetCurrentAmmoData() const
    {
        return CurrentAmmo;
    }
    bool TryToAddAmmo(int32 ClipsAmount);
    bool IsAmmoEmpty() const;
    virtual void Zoom(bool Enable)
    {

    }
  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent *WeaponMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleFlashSocket";
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Damage = 10.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float TraceMaxDistance = 20000.0f;
    UPROPERTY(EditAnywhere, Category = "Stats")
    FAmmoData DefaultAmmo{15, 10, false};
    UPROPERTY(EditAnywhere, Category = "UI")
    FWeaponUIData UIData;
    UPROPERTY(EditAnywhere, Category = "VFX")
    UNiagaraSystem *MuzzleFX;
    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue *FireSound;
    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue *NoAmmoSound;
    UPROPERTY(EditAnywhere, Category = "Sound")
    bool CanZoom;
    UPROPERTY(EditAnywhere, Category = "Sound", meta = (EditCondition = "CanZoom"))
    float NoZoomFOV = 90.0f;
    UPROPERTY(EditAnywhere, Category = "Sound", meta = (EditCondition = "CanZoom"))
    float ZoomFOV = 50.0f;
    AController *Controller;
    AController *GetController() const;
    bool GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation);
    FVector GetMuzzleWorldLocation() const;

    void MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd);

    virtual void BeginPlay() override;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
    virtual void MakeShot() {};

    UFUNCTION(Server, Reliable)
    virtual void MakeShotServer(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd, int32 InstigatorID);
    virtual void MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                               int32 InstigatorID);

    virtual void MakeShotFX(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd) {};



    UFUNCTION(NetMulticast, Reliable)
    virtual void MakeShotMulticast(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd, int32 InstigatorID);
    virtual void MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                  int32 InstigatorID);
    virtual FVector GetTraceData(FVector ViewLocation, FRotator ViewRotation) const;

    void DecreaseAmmo();
    
    bool isClipEmpty() const;
    bool IsAmmoFull() const;

    void LogAmmo();

    

    UNiagaraComponent *SpawnMuzzleFX();

  private:
    FAmmoData CurrentAmmo;
};
