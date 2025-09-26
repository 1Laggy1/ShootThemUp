// Shoot THem Up Game. All Rights Reserved.

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "STUUtils.h"
#include "Engine/DamageEvents.h"
ASTUProjectile::ASTUProjectile()
{
    SetReplicates(false);
    SetReplicateMovement(false);
    bReplicates = false;
    PrimaryActorTick.bCanEverTick = false;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    CollisionComponent->bReturnMaterialOnMove = true;
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("UProjectileMovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.3f;
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();
    check(MovementComponent);
    check(CollisionComponent);
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    CollisionComponent->IgnoreActorWhenMoving(GetOwner()->GetOwner(), true);
    CollisionComponent->IgnoreActorWhenMoving(this, true);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
    SetLifeSpan(LifeSeconds);
}

void ASTUProjectile::OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                                     UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    if (!GetWorld() || OtherActor == GetOwner())
        return;
    MovementComponent->StopMovementImmediately();

    //DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
    if (!WeaponFXComponent)
    {
        Destroy();
        return;
    }
    WeaponFXComponent->PlayImpactFX(Hit);
    if (!ExplosionSound)
    {
        Destroy();
        return;
    }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
    /*if (GetLocalRole() == ROLE_Authority)
    {
        UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius,
                                            UDamageType::StaticClass(), {}, this, GetController(), DoFullDamage);
    }*/

    FExplosionParams ExplosionParams;
    ExplosionParams.DamageRadius = DamageRadius;
    ExplosionParams.DamageAmount = DamageAmount;
    ExplosionParams.DamageAmountMin = DamageAmountMin;
    ExplosionParams.ExplosionStrength = ExplosionStrength;
    ExplosionParams.ExplosionStrengthMin = ExplosionStrengthMin;

    STUUtils::ApplyRadialDamageWithLineOfSight(GetWorld(), GetActorLocation(), ExplosionParams, this,
                                               Cast<ACharacter>(GetOwner())->Controller,
                                               {this});

    Destroy();
}
AController *ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
