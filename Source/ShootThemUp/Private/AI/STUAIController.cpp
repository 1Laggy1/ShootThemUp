// Shoot THem Up Game. All Rights Reserved.


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUHealthActorComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUPerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);
}

void ASTUAIController::BeginPlay()
{
    Super::BeginPlay();
    Cast<ASTUAICharacter>(GetPawn())->HealthComponent->OnDamaged.AddUObject(this, &ASTUAIController::OnDamaged);
}

void ASTUAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
    if (STUCharacter)
    {
        if (STUCharacter->BehaviorTreeAsset)
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
    }
}

void ASTUAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor *ASTUAIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent())
        return nullptr;

    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
void ASTUAIController::OnDamaged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                 class AController* InstigatedBy, AActor* DamageCauser)
{
    GetBlackboardComponent()->SetValueAsVector(LastEnemyPositionKey, InstigatedBy->GetPawn()->GetActorLocation());
}
