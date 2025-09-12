// Shoot THem Up Game. All Rights Reserved.

#include "Components/STUWeaponComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animations/AnimUtils.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerState.h"
#include "Weapon/STUBaseWeapon.h"

#include "STUCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr static int32 WeaponNum = 2;
void USTUWeaponComponent::Zoom(bool Enabled)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("Zoom"));
    if (CurrentWeapon)
    {
        CurrentWeapon->Zoom(Enabled);
    }
}
USTUWeaponComponent::USTUWeaponComponent()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("USTUWeaponComponent"));
    SetIsReplicatedByDefault(true);
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("BeginPlay"));
    Super::BeginPlay();
    // checkf(WeaponData.Num() == WeaponNum,
    // TEXT("Only exactly %i weapons on 1 character is allowed: Change Weapon data in WeaponComponent"), WeaponNum);
    CurrentWeaponIndex = 0;

    if (GetOwner()->HasAuthority())
    {
        SpawnWeapons();
        InitAnimations();
    }
}

void USTUWeaponComponent::SpawnWeapons_Implementation()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("SpawnWeapons_Implementation"));
    if (bWeaponsSpawned)
        return;
    bWeaponsSpawned = true;
    ASTUBaseCharacter *Character = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Character || !GetWorld())
        return;
    /*for (auto OneWeaponData : Character->SpawnInfo.WeaponClass)
    {*/
    FActorSpawnParameters Params;
    Params.Owner = Character;
    Params.Instigator = Character;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    UE_LOG(LogWeaponComponent, Display, TEXT("SpawnInfo: %s"), *Character->SpawnInfo.PlayerID);
    FPlayerInfo PlayerInfo = Character->SpawnInfo;
    ASTUBaseWeapon *Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(Character->SpawnInfo.WeaponClass,
                                                                    FVector::ZeroVector, FRotator::ZeroRotator, Params);
    if (!Weapon)
        return;

    // Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
    Weapon->SetOwner(Character);
    Weapons.Add(Weapon);
    AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);

    //}
    GetWeapons(0);
}
void USTUWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("GetLifetimeReplicatedProps"));
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USTUWeaponComponent, Weapons);
}

void USTUWeaponComponent::EquipWeaponServer_Implementation(int32 WeaponIndex, int32 InstigatedBy)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("EquipWeaponServer_Implementation"));
    EquipWeaponMulticast(WeaponIndex, InstigatedBy);
}

void USTUWeaponComponent::EquipWeaponMulticast_Implementation(int32 WeaponIndex, int32 InstigatedBy)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("EquipWeaponMulticast_Implementation"));
    if (InstigatedBy == Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
        return;

    EquipWeapon(WeaponIndex);
}

void USTUWeaponComponent::ReloadServer_Implementation(int32 InstigatedBy)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("ReloadServer_Implementation"));
    ReloadMulticast(InstigatedBy);
}

void USTUWeaponComponent::ReloadMulticast_Implementation(int32 InstigatedBy)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("ReloadMulticast_Implementation"));
    /*FString From = FString::FromInt(InstigatedBy);
    FString To = FString::FromInt(Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID());
    UE_LOG(LogWeaponComponent, Warning, TEXT("Requested reload anim: From %s to: %s"), *From, *To);*/
    if (!Cast<ACharacter>(GetOwner()) || !Cast<ACharacter>(GetOwner())->GetPlayerState() ||
        !Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
        return;
    if (InstigatedBy == Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
        return;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("EquipWeapon"));
    if (Weapons.IsEmpty() || WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Invalid weapon index, requested %d, length of weapons %d"), WeaponIndex,
               Weapons.Num());
        return;
    }
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    if (CurrentWeapon)
    {
        Zoom(false);
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeaponIndex = WeaponIndex;
    if (!Weapons[CurrentWeaponIndex])
    {
        return;
    }
    // FString WeaponIndexIs = Weapons[CurrentWeaponIndex] ? Weapons[CurrentWeaponIndex]->GetName() : "nullptr";
    // UE_LOG(LogTemp, Warning, TEXT("Weapons[CurrentWeaponIndex] is %s"), *WeaponIndexIs);
    CurrentWeapon = Weapons[CurrentWeaponIndex];
    // FString CurrentWeaponIndexIs = CurrentWeapon ? CurrentWeapon->GetName() : "nullptr";
    // UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon set to %s"), *CurrentWeaponIndexIs);
    //  CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;

    /*const auto CurrentWeaponData = WeaponData.FindByPredicate(
        [&](const FWeaponData &Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });*/
    CurrentReloadAnimMontage = CurrentWeapon->ReloadAnimMontage;
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    UE_LOG(LogWeaponComponent, Warning, TEXT("CURRENT WEAPON: %s"), *CurrentWeapon->GetFullName());
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon *Weapon, USkeletalMeshComponent *Mesh, FName SocketName)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("AttachWeaponToSocket"));
    if (!Weapon || !Mesh)
        return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(Mesh, AttachmentRules, SocketName);
}

void USTUWeaponComponent::StartFire()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("StartFire"));
    FString Result = CanFire() ? "Yes" : "No";
    UE_LOG(LogWeaponComponent, Display, TEXT("Start fire %s"), *Result);
    if (!CanFire())
        return;

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("StopFire"));
    if (!CurrentWeapon)
        return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("NextWeapon"));
    if (!CanEquip())
        return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
    EquipWeaponServer(CurrentWeaponIndex, Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID());
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("EndPlay"));
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
    Super::EndPlay(EndPlayReason);
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData &UIData) const
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("GetWeaponUIData"));

    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetCurrentAmmoData(FAmmoData &Data) const
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("GetCurrentAmmoData"));
    if (CurrentWeapon)
    {
        Data = CurrentWeapon->GetCurrentAmmoData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("TryToAddAmmo"));
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

void USTUWeaponComponent::GetWeapons(int32 MaxRetries)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("GetWeapons"));
    bool bAllWeaponsValid = true;
    for (auto Weapon : Weapons)
    {
        if (!Weapon || !IsValid(Weapon))
        {
            bAllWeaponsValid = false;
            break;
        }
    }
    if (!bAllWeaponsValid && MaxRetries > 0)
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Weapons not ready, retrying... (%d retries left)"), MaxRetries);

        FTimerHandle RetryTimer;
        GetWorld()->GetTimerManager().SetTimer(
            RetryTimer, [this, MaxRetries]() { GetWeapons(MaxRetries - 1); }, 0.1f, false);
        return;
    }

    if (!bAllWeaponsValid)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Failed to get valid weapons after all retries"));
        return;
    }

    // All weapons are valid, proceed
    for (auto Weapon : Weapons)
    {
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
    }
    EquipWeapon(CurrentWeaponIndex);
    InitAnimations();
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage *Animation)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("PlayAnimMontage"));
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("InitAnimations"));

    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify on weapon is not set"));
    }
    // for (auto OneWeaponData : WeaponData)
    //{
    auto ReloadFinishedNotify =
        AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(CurrentWeapon->ReloadAnimMontage);
    if (!ReloadFinishedNotify)
        return;

    ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    //}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent *Mesh)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("OnEquipFinished"));
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != Mesh)
        return;

    EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent *Mesh)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("OnReloadFinished"));
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != Mesh)
        return;

    ReloadAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire() const
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("CanFire"));

    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("CanEquip"));
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("CanReload"));
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon *AmmoEmptyWeapon)
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("OnEmptyClip"));
    if (!AmmoEmptyWeapon)
        return;
    if (CurrentWeapon == AmmoEmptyWeapon)
    {
        ChangeClip();
    }
    else
    {
        for (const auto Weapon : Weapons)
        {
            if (Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip();
            }
        }
    }
}
void USTUWeaponComponent::ChangeClip()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("ChangeClip"));
    if (!CanReload())
        return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);

    ReloadServer(Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID());
}

void USTUWeaponComponent::Reload()
{
    UE_LOG(LogWeaponComponent, Warning, TEXT("Reload"));
    ChangeClip();
}