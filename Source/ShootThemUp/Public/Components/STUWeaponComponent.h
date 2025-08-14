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
    void StartFire();
    void StopFire();
    void NextWeapon();
    void Reload();
    void EndPlay(const EEndPlayReason::Type EndPlayReason);
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
  private:
    UPROPERTY()
    ASTUBaseWeapon *CurrentWeapon = nullptr;
    int32 CurrentWeaponIndex = 0;
    bool EquipAnimInProgress;
    bool ReloadAnimInProgress;
    UPROPERTY()
    TArray<ASTUBaseWeapon *> Weapons = {};
    UPROPERTY()
    UAnimMontage *CurrentReloadAnimMontage = nullptr;
    void SpawnWeapons();
    void EquipWeapon(int32 WeaponIndex);
    

    void PlayAnimMontage(UAnimMontage *Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* Mesh);
    void OnReloadFinished(USkeletalMeshComponent *Mesh);
    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

    void OnEmptyClip();
    void ChangeClip();

    
};
