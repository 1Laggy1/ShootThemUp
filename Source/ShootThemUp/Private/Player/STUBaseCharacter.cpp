// Shoot THem Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Components/Abilities/STUDashAbilityComponent.h"
#include "Components/Abilities/STUPlayerAbilityUseComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/STUPlayerController.h"
#include "STUGameStateBase.h"
#include "STUUtils.h"
#include "Sound/SoundCue.h"
#include "UI/STUHealthBarWidget.h"
#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
    
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    bReplicateUsingRegisteredSubObjectList = true;
    HealthComponent = CreateDefaultSubobject<USTUHealthActorComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("Weapon Component");
    if (HealthComponent->GetHealthWidgetComponent())
    {
        HealthComponent->GetHealthWidgetComponent()->SetupAttachment(RootComponent);
    }
}
void ASTUBaseCharacter::Zoom(bool Enabled)
{
    if (IsSprinting() && Enabled)
    {
        StopSprint();
    }
    WeaponComponent->Zoom(Enabled);
}
void ASTUBaseCharacter::SetAbilityByActive()
{

    if (AbilityComponents.IsEmpty())
    {
        TArray<UActorComponent *> Components;
        GetComponents(USTUPlayerAbilityUseComponent::StaticClass(), Components);

        for (UActorComponent *Component : Components)
        {
            USTUPlayerAbilityUseComponent *AbilityComp = Cast<USTUPlayerAbilityUseComponent>(Component);
            if (AbilityComp)
            {
                AbilityComp->SetActive(false);
                AbilityComponents.Add(AbilityComp);
            }
        }
    }

    for (USTUPlayerAbilityUseComponent *AbilityOne : AbilityComponents)
    {
        if (AbilityOne && AbilityOne->GetClass() == AbilityClass)
        {
            ActiveAbilityComponent = AbilityOne;
            AbilityOne->SetActive(true);
            return;
        }
    }
}

void ASTUBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTUBaseCharacter, PlayerID);
    DOREPLIFETIME(ASTUBaseCharacter, PlayerName);
    DOREPLIFETIME(ASTUBaseCharacter, PlayerColor);
    DOREPLIFETIME(ASTUBaseCharacter, MovementEnabled);
    DOREPLIFETIME(ASTUBaseCharacter, ActiveAbilityComponent);
    DOREPLIFETIME(ASTUBaseCharacter, AbilityClass);
}

void ASTUBaseCharacter::OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser)
{
    if (HealthComponent->isDead() || !GetController())
        return;

    // UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageSound, GetActorLocation());
}
void ASTUBaseCharacter::InitPlayer()
{
    SetPlayerColor(PlayerColor);
    if (GetWorld() && UGameplayStatics::GetCurrentLevelName(GetWorld()) == "LobbyLevel")
    {
        return;
    }
    if (!GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->PlayerState)
    {
        return;
    }
    if (PlayerID == GetWorld()->GetFirstPlayerController()->PlayerState->GetUniqueId()->ToString())
    {
        UE_LOG(BaseCharacterLog, Display, TEXT("Yes! Requesting possess now"));
        Cast<ASTUPlayerController>(GetWorld()->GetFirstPlayerController())->RequestPossess_Server(this);
    }
    InitAbility();
}
void ASTUBaseCharacter::InitAbility()
{
}
// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    SetAbilityByActive();
    if (HealthComponent)
    {
        OnHealthChanged(HealthComponent->GetHealth());
        UE_LOG(LogTemp, Warning, TEXT("HealthComponent pointer: %p, owner: %s"), HealthComponent,
               *GetNameSafe(HealthComponent->GetOwner()));
        UE_LOG(LogTemp, Warning, TEXT("Character pointer: %p, name: %s"), this, *GetNameSafe(this));

        HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
        HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
        HealthComponent->OnDamaged.AddUObject(this, &ASTUBaseCharacter::OnDamaged);
    }

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), RespawnSound, GetActorLocation());
    InitPlayer();
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor &Color)
{
    const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (!MaterialInst)
        return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (GetCharacterMovement()->Velocity.Length() > 5)
    {
        isWalking = true;
    }
    else
    {
        isWalking = false;
    }
}

void ASTUBaseCharacter::StartFire()
{
    bWantToShoot = true;
    if (IsSprinting())
    {
        StopSprint();
    }
    WeaponComponent->StartFire();
}

void ASTUBaseCharacter::StopFire()
{
    bWantToShoot = false;
    WeaponComponent->StopFire();
}


void ASTUBaseCharacter::StartSprint()
{
    bIsLocallySprinting = true;
    Zoom(false);
    WeaponComponent->StopFire();
    RequestSprintServer(true);
}

void ASTUBaseCharacter::StopSprint()
{
    bIsLocallySprinting = false;
    RequestSprintServer(false);
}

void ASTUBaseCharacter::MulticastStartSprint_Implementation(bool Start)
{
    if (Start)
    {
        if (!isSprintingPressed)
        {
            isSprintingPressed = true;
            GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
        }
    }
    else
    {
        if (isSprintingPressed)
        {
            isSprintingPressed = false;
            GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
        }
    }
}

void ASTUBaseCharacter::RequestSprintServer_Implementation(bool Start)
{
    MulticastStartSprint(Start);
}

bool ASTUBaseCharacter::IsSprinting()
{
    return isSprintingPressed && isWalking;
}

FRotator ASTUBaseCharacter::CalculateAimRotation()
{

    float Pitch = GetBaseAimRotation().Pitch;
    if (Pitch > 180)
    {
        Pitch -= 360;
    }

    AimRotation = GetBaseAimRotation();
    AimRotation.Pitch = Pitch;
    return AimRotation;
    // UE_LOG(BaseCharacterLog, Warning, TEXT("Player: %s. Tick AimRotation: %s"), *GetActorNameOrLabel(),
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
        return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degress = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degress : Degress * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanTime);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    // HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::UpdateHealthWidgetVisibility(AActor *DamageCauser)
{
}

void ASTUBaseCharacter::Landed(const FHitResult &Hit)
{
    Super::Landed(Hit);
    /*const auto FallVelocityz = -GetVelocity().Z;
    if (FallVelocityz < LandedDamageVelocity.X)
        return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityz);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);*/
}
