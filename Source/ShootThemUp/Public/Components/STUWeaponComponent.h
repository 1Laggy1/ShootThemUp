// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUWeaponComponent();
    void Fire();

  protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;
    virtual void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAttachPointName = "WeaponPointer";

  private:
    UPROPERTY()
    ASTUBaseWeapon *CurrentWeapon = nullptr;
    void SpawnWeapon();
};
