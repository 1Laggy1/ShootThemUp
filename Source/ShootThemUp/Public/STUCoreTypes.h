#pragma once
#include "STUCoreTypes.generated.h"
// weapon
class ASTUBaseWeapon;
DECLARE_MULTICAST_DELEGATE(FOnClipSignature);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "0"))
    int32 Bullets;
    UPROPERTY(EditAnywhere, Category = "Stats", meta = (EditCondition = "!Infinite"))
    int32 Clips;
    UPROPERTY(EditAnywhere, Category = "Stats")
    bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage *ReloadAnimMontage;
};

// health
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)
