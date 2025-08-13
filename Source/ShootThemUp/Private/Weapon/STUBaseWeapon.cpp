// Shoot THem Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include <Components/STUHealthActorComponent.h>
#include "GameFramework/Actor.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"



DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    Controller = GetPlayerController();
    check(WeaponMesh);
}

void ASTUBaseWeapon::Fire()
{
    MakeShot();
}

void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld())
        return;

    if (Controller == nullptr)
    {
        Controller = GetPlayerController();
        if (Controller == nullptr)
            return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    const FVector SocketLocation = GetMuzzleWorldLocation();


    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(), SocketLocation, HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        if (HitResult.GetActor())
        {
            MakeDamage(HitResult.GetActor());
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), SocketLocation, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}

APlayerController *ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const
{
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd)
{
    if (!GetWorld())
        return;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
                                         ECollisionChannel::ECC_Visibility, CollisionParams); // CollisionParams);
}

void ASTUBaseWeapon::MakeDamage(AActor* ActorToHit)
{
    ActorToHit->TakeDamage(Damage, FDamageEvent(), Controller, this);
}

