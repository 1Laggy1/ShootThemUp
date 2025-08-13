// Shoot THem Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.

    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthActorComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->bOwnerNoSee = true;

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("Weapon Component");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// aCalled to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ASTUBaseCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ASTUBaseCharacter::StopSprint);
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent,
                                     &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", EInputEvent ::IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    if (Amount > 0 && GetCharacterMovement()->Velocity.Length() > 5)
    {
        isWalking = true;
    }
    else
    {
        isWalking = false;
    }
    AddMovementInput(GetActorForwardVector(), Amount);
}

// Called every frame
void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::StartSprint()
{
    isSprintingPressed = true;
    GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier; // Double the speed when sprinting
}

void ASTUBaseCharacter::StopSprint()
{
    isSprintingPressed = false;
    GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier; // Reset the speed when stopping sprint
}

bool ASTUBaseCharacter::IsSprinting()
{
    return isSprintingPressed && isWalking; // Check if the character is both sprinting and walking
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
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());
    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanTime);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
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
