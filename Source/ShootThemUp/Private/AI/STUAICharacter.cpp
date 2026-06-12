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
DEFINE_LOG_CATEGORY_STATIC(STUAICharacter, All, All);

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("Weapon Component"))
{
    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
    
}
void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();
    HealthComponent = FindComponentByClass<USTUHealthActorComponent>();
    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddUObject(this, &ASTUAICharacter::OnDeath);
        HealthComponent->OnHealthChanged.AddUObject(this, &ASTUAICharacter::OnHealthChanged);
        HealthComponent->OnDamaged.AddUObject(this, &ASTUAICharacter::OnDamaged);
    }
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
    UE_LOG(STUAICharacter, Display, TEXT("Bot %s is dead"), *GetName());
}

void ASTUAICharacter::OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser)
{
    Super::OnDamaged(DamagedActor, Damage, DamageCauser);
    if (DamageCauser)
    {
        UAISense_Damage::ReportDamageEvent(GetWorld(), Cast<AActor>(GetOwner()), DamageCauser, Damage,
                                           DamageCauser->GetActorLocation(),
                                           Cast<AActor>(GetOwner())->GetActorLocation());
    }

    
}

void ASTUAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    
}

void ASTUAICharacter::OnHealthChanged(float Health)
{
    Super::OnHealthChanged(Health);

}

