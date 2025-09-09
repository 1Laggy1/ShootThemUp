// Shoot THem Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "Weapon/STUBaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"
#include "UI/STUHealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Player/STUPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.

    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true; 
    HealthComponent = CreateDefaultSubobject<USTUHealthActorComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("Weapon Component");
    if (HealthComponent->GetHealthWidgetComponent())
    {
        HealthComponent->GetHealthWidgetComponent()->SetupAttachment(RootComponent);
    }
    


}

void ASTUBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    //DOREPLIFETIME(USTUWeaponComponent, WeaponComponent->bWeaponsSpawned);
}

void ASTUBaseCharacter::OnDamaged(AActor *DamagedActor, float Damage, AActor* DamageCauser)
{
    if (HealthComponent->isDead() || !GetController())
        return;

     

    //UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageSound, GetActorLocation());
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
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

// aCalled to bind functionality to input

void ASTUBaseCharacter::StartSprint()
{
    RequestSprintServer(true);
}

void ASTUBaseCharacter::StopSprint()
{
    RequestSprintServer(false);
}


void ASTUBaseCharacter::MulticastStartSprint_Implementation(bool Start)
{
    if (Start)
    {
        if (!isSprintingPressed)
        {
            isSprintingPressed = true;
            GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier; // Double the speed when sprinting
        }
    }
    else
    {
        if (isSprintingPressed)
        {
            isSprintingPressed = false;
            GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier; // Reset the speed when stopping sprint
        }
    }
}

void ASTUBaseCharacter::RequestSprintServer_Implementation(bool Start)
{
    MulticastStartSprint(Start);
}


bool ASTUBaseCharacter::IsSprinting()
{
    return isSprintingPressed && isWalking; // Check if the character is both sprinting and walking
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
    //UE_LOG(BaseCharacterLog, Warning, TEXT("Player: %s. Tick AimRotation: %s"), *GetActorNameOrLabel(),
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
    //HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::UpdateHealthWidgetVisibility(AActor *DamageCauser)
{

    
    
}

void ASTUBaseCharacter::Landed(const FHitResult &Hit)
{
    Super::Landed(Hit);
    const auto FallVelocityz = -GetVelocity().Z;
    if (FallVelocityz < LandedDamageVelocity.X)
        return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityz);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}
