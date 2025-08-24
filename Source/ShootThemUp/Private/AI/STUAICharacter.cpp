// Shoot THem Up Game. All Rights Reserved.

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "Perception/AISense_Damage.h"
#include "Components/STUAIWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("Weapon Component"))
{
    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
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
    
}
