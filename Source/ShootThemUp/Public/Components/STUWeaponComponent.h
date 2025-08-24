// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUWeaponComponent();
    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void Reload();
    void EndPlay(const EEndPlayReason::Type EndPlayReason);
    bool GetWeaponUIData(FWeaponUIData &UIData) const;
    bool GetCurrentAmmoData(FAmmoData &Data) const;
    bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);
  protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData = {};
    virtual void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponPoint";
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage *EquipAnimMontage;
    void AttachWeaponToSocket(ASTUBaseWeapon *Weapon, USkeletalMeshComponent *Mesh, FName SocketName);
    bool CanFire() const;
    bool CanEquip() const;
    ASTUBaseWeapon *CurrentWeapon = nullptr;
    int32 CurrentWeaponIndex = 0;
    UPROPERTY()
    TArray<ASTUBaseWeapon *> Weapons = {};
    void EquipWeapon(int32 WeaponIndex);
  private:
    UPROPERTY()
    
    
    bool EquipAnimInProgress;
    bool ReloadAnimInProgress;
    
    UPROPERTY()
    UAnimMontage *CurrentReloadAnimMontage = nullptr;
    void SpawnWeapons();
   
    

    void PlayAnimMontage(UAnimMontage *Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* Mesh);
    void OnReloadFinished(USkeletalMeshComponent *Mesh);
   
    bool CanReload() const;

    void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();

    
};
