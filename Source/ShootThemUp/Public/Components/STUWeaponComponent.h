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
    void Zoom(bool Enabled);
    bool bWeaponsSpawned;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> DefaultWeaponClass;
  protected:
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
    UFUNCTION()
    void OnRep_Weapons()
    {
        GetWeapons(MaxRetriesForLoops);
        EquipWeapon(CurrentWeaponIndex);
    }

    UPROPERTY(ReplicatedUsing = OnRep_Weapons)
    TArray<ASTUBaseWeapon *> Weapons = {};
    /*UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData = {};*/
    virtual void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponPoint";
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage *EquipAnimMontage;
    int32 MaxRetriesForLoops = 10;
    void AttachWeaponToSocket(ASTUBaseWeapon *Weapon, USkeletalMeshComponent *Mesh, FName SocketName);
    bool CanFire() const;
    bool CanEquip() const;
    ASTUBaseWeapon *CurrentWeapon = nullptr;
    int32 CurrentWeaponIndex = 0;
    UFUNCTION(Server, Reliable)
    void EquipWeaponServer(int32 WeaponIndex, int32 InstigatedBy);
    UFUNCTION(Server, Reliable)
    void ReloadServer(int32 InstigatedBy);
    UFUNCTION(NetMulticast, Reliable)
    void ReloadMulticast(int32 InstigatedBy);
    UFUNCTION(NetMulticast, Reliable)
    void EquipWeaponMulticast(int32 WeaponIndex, int32 InstigatedBy);
    void EquipWeapon(int32 WeaponIndex);
    
  private:
    
    
    bool EquipAnimInProgress;
    bool ReloadAnimInProgress;
    
    UPROPERTY()
    UAnimMontage *CurrentReloadAnimMontage = nullptr;
    UFUNCTION(Server, Reliable)
    void SpawnWeapons();
    void GetWeapons(int32 MaxRetries);
    

    void PlayAnimMontage(UAnimMontage *Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* Mesh);
    void OnReloadFinished(USkeletalMeshComponent *Mesh);
   
    bool CanReload() const;

    void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();

    
};
