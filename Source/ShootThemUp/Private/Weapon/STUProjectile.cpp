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

    TArray<FOverlapResult> Overlaps;
    TArray<AActor*> FinishedActors;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(DamageRadius);
    if (GetWorld()->OverlapMultiByChannel(Overlaps, GetActorLocation(), FQuat::Identity, ECC_PhysicsBody, Sphere))
    {
        for (auto &Result : Overlaps)
        {
            UPrimitiveComponent *OverlappedComp = Result.GetComponent();
            AActor *OverlappedActor = Result.GetActor();
            if (FinishedActors.Contains(OverlappedActor))
                continue;

            if (ACharacter *HitCharacter = Cast<ACharacter>(OverlappedActor))
            {
                FVector Direction = HitCharacter->GetActorLocation() - GetActorLocation();
                float Damage = FMath::GetMappedRangeValueClamped(FVector2D(0.f, DamageRadius), FVector2D(DamageAmount, DamageAmountMin), Direction.Size());
                FPointDamageEvent DEvent;
                DEvent.DamageTypeClass = UDamageType::StaticClass();
                HitCharacter->TakeDamage(Damage, DEvent, Cast<ACharacter>(GetOwner())->Controller, this);
                float ExplosionStrengthMapped = FMath::GetMappedRangeValueClamped(
                    FVector2D(0.f, DamageRadius), FVector2D(ExplosionStrength, ExplosionStrengthMin), Direction.Size());
                
                Direction.Normalize();
                FVector LaunchVelocity = Direction * ExplosionStrengthMapped;
                LaunchVelocity.Z += ExplosionStrengthMapped/3;

                HitCharacter->LaunchCharacter(LaunchVelocity, true, true);
            }
            else if (OverlappedComp && OverlappedComp->IsSimulatingPhysics())
            {
                OverlappedComp->AddRadialImpulse(GetActorLocation(), DamageRadius, ExplosionStrength,
                                                 ERadialImpulseFalloff::RIF_Linear, true);
            }
            FinishedActors.Add(OverlappedActor);
        }
    }

    Destroy();
}
AController *ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
