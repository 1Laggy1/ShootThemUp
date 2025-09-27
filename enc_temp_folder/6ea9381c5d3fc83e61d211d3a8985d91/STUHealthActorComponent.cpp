// Shoot THem Up Game. All Rights Reserved.

#include "Components/STUHealthActorComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Player/STUPlayerController.h"
#include "Player/STUPlayerState.h"
#include "STUGameModeBase.h"
#include "STUUtils.h"

#include "UI/STUHealthBarWidget.h"
DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthActorComponent::USTUHealthActorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Health = MaxHealth;
    SetIsReplicatedByDefault(true);
    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

void USTUHealthActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (GetOwner() && GetOwner()->GetLocalRole() == ROLE_Authority)
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
    HealthChangedMulticast(Health);
    // OnHealthChanged.Broadcast(Health);
    return Health;
}

void USTUHealthActorComponent::UpdateHealthWidget(AActor *DamageCauser, float NewHealth)
{
    if (!GetOwner() || !HealthWidgetComponent)
        return;

    if (DamageCauser)
    {
        const auto ControllerCauser = STUUtils::GetInstigatorControllerFromDamageCauser(DamageCauser);
        if (ControllerCauser)
        {
            const auto PlayerLocation = DamageCauser->GetActorLocation();
            const auto Distance = FVector::Distance(PlayerLocation, GetOwner()->GetActorLocation());
            HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
        }
    }

    if (!HealthBarWidget)
    {
        HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
        if (!HealthBarWidget)
            return;
    }
    //if (DamageCauser == GetWorld()->GetFirstPlayerController())
    HealthBarWidget->SetHealthPercent(NewHealth/MaxHealth);

    /*FString yes = HealthBarWidget->IsVisible() ? FString("YES") : FString("NO");
    UE_LOG(LogTemp, Warning, TEXT("Visibility of HEALTH WIDGET COMPONENT = %s HEALTH PERCENT IS %s"), *yes,
           *FString::Printf(TEXT("%.2f"), GetHealthPercent()));*/
}

void USTUHealthActorComponent::BeginPlay()
{
    Super::BeginPlay();

    HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    // Health = MaxHealth;
    // HealthChangedMulticast(Health);
    // OnHealthChanged.Broadcast(Health);
    AActor *ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthActorComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthActorComponent::OnTakeRadialDamage);
    }
}

void USTUHealthActorComponent::ApplyDamageServer_Implementation(AActor *DamagedActor, float Damage,
                                                                AActor *DamageCauser)
{
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    ApplyDamageMulticast(DamagedActor, Damage, DamageCauser, Health);
    if (isDead())
    {
        if (!Cast<ACharacter>(GetOwner()) || !Cast<ACharacter>(GetOwner())->GetPlayerState() ||
            !Cast<ACharacter>(GetOwner())->GetPlayerState()->GetUniqueID())
            return;
        const auto ControllerVictim = Cast<ACharacter>(GetOwner())->Controller;
        const auto ControllerCauser = Cast<ACharacter>(DamageCauser)->Controller;
        // OnDeath.Broadcast();

        DeathMulticast(Cast<ACharacter>(DamagedActor)->GetPlayerState()->GetUniqueID());

        Killed(ControllerCauser, ControllerVictim);
    }
}

void USTUHealthActorComponent::ApplyDamageMulticast_Implementation(AActor *DamagedActor, float Damage,
                                                                   AActor *DamageCauser, float NewHealth)
{
    const auto ControllerCauser = STUUtils::GetInstigatorControllerFromDamageCauser(DamageCauser);
    if (ControllerCauser == GetWorld()->GetFirstPlayerController())
    {
        UpdateHealthWidget(DamageCauser, NewHealth);
        OnDamaged.Broadcast(DamagedActor, Damage, DamageCauser);
        OnHealthChanged.Broadcast(NewHealth);
    }
    HealDelayCurrent = 0;
    IsVaunded = true;
}

void USTUHealthActorComponent::DeathMulticast_Implementation(int32 PlayerID)
{
    HealthWidgetComponent->SetVisibility(false, true);
    OnDeath.Broadcast();
}

void USTUHealthActorComponent::HealthChangedMulticast_Implementation(float NewHealth)
{
    UpdateHealthWidget(nullptr, NewHealth);
    OnHealthChanged.Broadcast(NewHealth);
}

void USTUHealthActorComponent::OnTakePointDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy,
                                                 FVector HitLocation, UPrimitiveComponent *FHitComponent,
                                                 FName BoneName, FVector ShotFromDirection,
                                                 const UDamageType *DamageType, AActor *DamageCauser)
{
    if (!DamagedActor || !GetOwner())
        return;
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
    UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final: %f, bone: %s"), Damage, FinalDamage,
           *BoneName.ToString());
    if (GetOwner()->GetLocalRole() == ROLE_Authority)
    {
        if (InstigatedBy)
            ApplyDamage(DamagedActor, Damage, InstigatedBy, DamageType, InstigatedBy->GetPawn());
        else
            ApplyDamage(DamagedActor, Damage, nullptr, DamageType, nullptr);
    }
}

void USTUHealthActorComponent::OnTakeRadialDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                                  FVector Origin, const FHitResult &HitInfo, AController *InstigatedBy,
                                                  AActor *DamageCauser)
{
    UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
    if (GetOwner()->GetLocalRole() == ROLE_Authority)
    {
        if (InstigatedBy)
            ApplyDamage(DamagedActor, Damage, InstigatedBy, DamageType, InstigatedBy->GetPawn());
        else
            ApplyDamage(DamagedActor, Damage, nullptr, DamageType, nullptr);
    }
}

void USTUHealthActorComponent::ApplyDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy,
                                           const UDamageType *DamageType, AActor *DamageCauser)
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
