// Shoot THem Up Game. All Rights Reserved.

#include "Components/STUHealthActorComponent.h"
#include "GameFramework/Character.h"
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"
#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthActorComponent::USTUHealthActorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Health = MaxHealth;
    SetIsReplicatedByDefault(true);
}

void USTUHealthActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    AutoHealHandle(DeltaTime);
    
}

void USTUHealthActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USTUHealthActorComponent, Health);
}

float USTUHealthActorComponent::TakeHeal(float amount)
{
    if (amount <= 0 || isDead())
    {
        return Health;
    }
    Health = FMath::Min(Health + amount, MaxHealth);
    if (FMath::IsNearlyEqual(Health, MaxHealth))
    {
        IsVaunded = false;
    }
    OnHealthChanged.Broadcast(Health);
    return Health;
}

void USTUHealthActorComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);
    AActor *ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthActorComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthActorComponent::OnTakeRadialDamage);
    }
}



void USTUHealthActorComponent::ApplyDamageServer_Implementation(AActor* DamagedActor, float Damage, AActor* DamageCauser)
{
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    ApplyDamageMulticast(DamagedActor, Damage, DamageCauser);
    if (isDead())
    {
        if (!Cast<ACharacter>(GetOwner()) || !Cast<ACharacter>(GetOwner())->GetPlayerState() ||
            !Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
            return;
        const auto ControllerVictim = Cast<ACharacter>(GetOwner())->Controller;
        const auto ControllerCauser = Cast<ACharacter>(DamageCauser)->Controller;
        //OnDeath.Broadcast();
        
        DeathMulticast(Cast<ACharacter>(DamagedActor)->GetPlayerState()->GetUniqueID());

        Killed(ControllerCauser, ControllerVictim);
    }
}

void USTUHealthActorComponent::ApplyDamageMulticast_Implementation(AActor *DamagedActor, float Damage,
                                                                   AActor *DamageCauser)
{
    //Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    HealDelayCurrent = 0;
    OnDamaged.Broadcast(DamagedActor, Damage, DamageCauser);
    IsVaunded = true;
}

void USTUHealthActorComponent::DeathMulticast_Implementation(int32 PlayerID)
{
    OnDeath.Broadcast();
}

void USTUHealthActorComponent::OnTakePointDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy,
                                                 FVector HitLocation, UPrimitiveComponent *FHitComponent,
                                                 FName BoneName, FVector ShotFromDirection,
                                                 const UDamageType *DamageType, AActor *DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
    UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final: %f, bone: %s"), Damage, FinalDamage,
           *BoneName.ToString());
    if (GetOwner()->GetLocalRole() == ROLE_Authority)
    ApplyDamage(DamagedActor, Damage, InstigatedBy, DamageType, InstigatedBy->GetPawn());
}

void USTUHealthActorComponent::OnTakeRadialDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                                  FVector Origin, const FHitResult &HitInfo, AController *InstigatedBy,
                                                  AActor *DamageCauser)
{
    UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
    if (GetOwner()->GetLocalRole() == ROLE_Authority)
    ApplyDamage(DamagedActor, Damage, InstigatedBy, DamageType, InstigatedBy->GetPawn());
}

void USTUHealthActorComponent::ApplyDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy,
                                           const UDamageType *DamageType,
                                           AActor *DamageCauser)
{
    ApplyDamageServer(DamagedActor, Damage, DamageCauser);
   
}
float USTUHealthActorComponent::GetPointDamageModifier(AActor *DamagedActor, const FName &BoneName)
{
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName) ||
        !Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial())
        return 1.0f;

    const auto PhysicalMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!DamageModifiers.Contains(PhysicalMaterial))
        return 1.0f;

    return DamageModifiers[PhysicalMaterial];

}

void USTUHealthActorComponent::AutoHealHandle(float DeltaTime)
{
    if (!AutoHeal || !IsVaunded)
    {
        return;
    }
        

    if (HealDelayCurrent >= HealDelay)
    {
        TakeHeal(HealAmount * DeltaTime);
    }
    else
    {
        HealDelayCurrent += DeltaTime;
    }
}

void USTUHealthActorComponent::Killed(AController *KillerActor, AController *DiedActor)
{
    if (!GetWorld())
        return;
    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return;
    GameMode->Killed(KillerActor, DiedActor);
}


