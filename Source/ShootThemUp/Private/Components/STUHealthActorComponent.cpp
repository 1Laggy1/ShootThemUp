// Shoot THem Up Game. All Rights Reserved.

#include "Components/STUHealthActorComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthActorComponent::USTUHealthActorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
}

void USTUHealthActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    AutoHealHandle(DeltaTime);
    
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
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthActorComponent::OnTakeAnyDamage);
    }
}


void USTUHealthActorComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                               AController *InstigatedBy, AActor *DamageCauser)
{
    if (Damage <= 0 || isDead())
    {
        return;
    }
    HealDelayCurrent = 0;
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
    OnDamaged.Broadcast();
    IsVaunded = true;

    if (isDead())
    {
        OnDeath.Broadcast();
    }

    
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
