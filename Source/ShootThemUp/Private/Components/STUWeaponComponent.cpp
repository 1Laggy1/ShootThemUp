// Shoot THem Up Game. All Rights Reserved.

#include "Components/STUWeaponComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animations/AnimUtils.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/STUBaseWeapon.h"
#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr static int32 WeaponNum = 2;
void USTUWeaponComponent::Zoom(bool Enabled)
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Zoom(Enabled);
    }
}
USTUWeaponComponent::USTUWeaponComponent()
{
    SetIsReplicatedByDefault(true);
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    checkf(WeaponData.Num() == WeaponNum,
           TEXT("Only exactly %i weapons on 1 character is allowed: Change Weapon data in WeaponComponent"), WeaponNum);
    CurrentWeaponIndex = 0;
    InitAnimations();
    if (GetOwner()->HasAuthority())
    {
        SpawnWeapons();
    }
}

void USTUWeaponComponent::SpawnWeapons_Implementation()
{
    if (bWeaponsSpawned)
        return;
    bWeaponsSpawned = true;
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld())
        return;
    for (auto OneWeaponData : WeaponData)
    {
        FActorSpawnParameters Params;
        Params.Owner = Character;
        Params.Instigator = Character;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ASTUBaseWeapon *Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass, FVector::ZeroVector,
                                                                        FRotator::ZeroRotator, Params);
        if (!Weapon)
            continue;

        //Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
        
    }
    GetWeapons();
    
}
void USTUWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USTUWeaponComponent, Weapons);
}

void USTUWeaponComponent::EquipWeaponServer_Implementation(int32 WeaponIndex, int32 InstigatedBy)
{
    EquipWeaponMulticast(WeaponIndex, InstigatedBy);
}

void USTUWeaponComponent::EquipWeaponMulticast_Implementation(int32 WeaponIndex, int32 InstigatedBy)
{
    if (InstigatedBy == Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
        return;

    EquipWeapon(WeaponIndex);
}

void USTUWeaponComponent::ReloadServer_Implementation(int32 InstigatedBy)
{
    ReloadMulticast(InstigatedBy);
}

void USTUWeaponComponent::ReloadMulticast_Implementation(int32 InstigatedBy)
{
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
    if (!Weapons[CurrentWeaponIndex])
    {
        return;
    }
    //FString WeaponIndexIs = Weapons[CurrentWeaponIndex] ? Weapons[CurrentWeaponIndex]->GetName() : "nullptr"; 
    //UE_LOG(LogTemp, Warning, TEXT("Weapons[CurrentWeaponIndex] is %s"), *WeaponIndexIs);
    CurrentWeapon = Weapons[CurrentWeaponIndex];
    //FString CurrentWeaponIndexIs = CurrentWeapon ? CurrentWeapon->GetName() : "nullptr"; 
    //UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon set to %s"), *CurrentWeaponIndexIs);
    // CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;

    const auto CurrentWeaponData = WeaponData.FindByPredicate(
        [&](const FWeaponData &Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon *Weapon, USkeletalMeshComponent *Mesh, FName SocketName)
{
    if (!Weapon || !Mesh)
        return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(Mesh, AttachmentRules, SocketName);
}

void USTUWeaponComponent::StartFire()
{
    FString Result = CanFire() ? "Yes" : "No";
    UE_LOG(LogWeaponComponent, Display, TEXT("Start fire %s"), *Result);
    if (!CanFire())
        return;

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    if (!CanEquip())
        return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
    EquipWeaponServer(CurrentWeaponIndex, Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID());
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
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

    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetCurrentAmmoData(FAmmoData &Data) const
{
    if (CurrentWeapon)
    {
        Data = CurrentWeapon->GetCurrentAmmoData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

void USTUWeaponComponent::GetWeapons_Implementation()
{
    for (auto Weapon : Weapons)
    {
        if (!Weapon) continue;
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
    }
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage *Animation)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{

    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify on weapon is not set"));
    }
    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify =
            AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify)
            continue;

        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent *Mesh)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != Mesh)
        return;

    EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent *Mesh)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != Mesh)
        return;

    ReloadAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire() const
{

    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon *AmmoEmptyWeapon)
{
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
    ChangeClip();
}