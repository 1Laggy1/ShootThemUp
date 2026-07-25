// Shoot THem Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include <Components/STUHealthActorComponent.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{

    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(SceneRoot);
    
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    Controller = GetController();
    check(WeaponMesh);
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASTUBaseWeapon::StartFire()
{
}
void ASTUBaseWeapon::StopFire()
{
}
void ASTUBaseWeapon::MakeShotServer_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                   int32 InstigatorID)
{
}

void ASTUBaseWeapon::MakeShotMulticast_Implementation(FVector ViewLocation, FRotator ViewRotation, FVector TraceEnd,
                                                      int32 InstigatorID)
{
}

AController *ASTUBaseWeapon::GetController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return nullptr;

    return Player->GetController<AController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation)
{

    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if (!STUCharacter || !STUCharacter->Controller)
        return false;

    if (STUCharacter->IsPlayerControlled())
    {
        STUCharacter->Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
        FVector TraceEnd = GetTraceData(ViewLocation, ViewRotation);
        FHitResult HitResult;
        MakeHit(HitResult, ViewLocation, TraceEnd);
        ViewLocation = GetMuzzleWorldLocation();
        if (HitResult.bBlockingHit)
        ViewRotation = (HitResult.ImpactPoint - ViewLocation).Rotation();
    }
    else
    {
        ViewLocation = GetMuzzleWorldLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }
    
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FVector ASTUBaseWeapon::GetTraceData(FVector ViewLocation, FRotator ViewRotation) const
{
    const FVector ShootDirection = ViewRotation.Vector();
    FVector TraceEnd = ViewLocation + ShootDirection * TraceMaxDistance;

    return TraceEnd;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("No more bullets in clip"));
        return;
    }
    CurrentAmmo.Bullets--;

    if (isClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && isClipEmpty();
}

bool ASTUBaseWeapon::isClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

void ASTUBaseWeapon::ChangeClip()
{

    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}
bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
        return false;

    if (IsAmmoEmpty())
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty: "));
        LogAmmo();
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            UE_LOG(LogBaseWeapon, Display, TEXT("Only clips were added, was:"));
            LogAmmo();
            CurrentAmmo.Clips = NextClipsAmount;
        }
        else
        {
            UE_LOG(LogBaseWeapon, Display, TEXT("Full of ammo, was: "));
            LogAmmo();
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        }
    }
    else
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Only bullets added, was: "));
        LogAmmo();
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    }
    UE_LOG(LogBaseWeapon, Display, TEXT("Reffiled Ammo:"));
    LogAmmo();
    return true;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}


UNiagaraComponent *ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,              //
                                                 WeaponMesh,            //
                                                 MuzzleSocketName,      //
                                                 FVector::ZeroVector,   //
                                                 FRotator::ZeroRotator, //
                                                 EAttachLocation::SnapToTarget, true);
    
}

void ASTUBaseWeapon::MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd)
{
    if (!GetWorld())
        return;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.bReturnPhysicalMaterial = true;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility,
                                         CollisionParams);
}

