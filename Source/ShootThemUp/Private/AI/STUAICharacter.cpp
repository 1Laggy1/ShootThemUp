// Shoot THem Up Game. All Rights Reserved.

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "Perception/AISense_Damage.h"
#include "Components/STUAIWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/STUHealthActorComponent.h"
#include "UI/STUHealthBarWidget.h"
DEFINE_LOG_CATEGORY_STATIC(STUAICharacter, All, All);

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("Weapon Component"))
{
    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}
void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HealthWidgetComponent);
}
void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();

    if (AAIController *AIController = Cast<AAIController>(Controller))
    {
        if (UBrainComponent *Brain = AIController->GetBrainComponent())
        {
            Brain->StopLogic(TEXT("Character is dead"));
            Brain->Cleanup();
        }
    }
    HealthWidgetComponent->SetVisibility(false, true);
    UE_LOG(STUAICharacter, Display, TEXT("Bot %s is dead"), *GetName());
}

void ASTUAICharacter::OnDamaged(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                                class AController *InstigatedBy, AActor *DamageCauser)
{
    Super::OnDamaged(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
    if (InstigatedBy)
    {
        UAISense_Damage::ReportDamageEvent(GetWorld(), Cast<AActor>(GetOwner()), DamageCauser, Damage,
                                           DamageCauser->GetActorLocation(),
                                           Cast<AActor>(GetOwner())->GetActorLocation());
    }

    
}

void ASTUAICharacter::Tick(float DeltaTime)
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

void ASTUAICharacter::OnHealthChanged(float Health)
{
    Super::OnHealthChanged(Health);

    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget)
        return;
    UpdateHealthWidgetVisibility();
    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawn() || !HealthWidgetComponent)
        return;

    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}
