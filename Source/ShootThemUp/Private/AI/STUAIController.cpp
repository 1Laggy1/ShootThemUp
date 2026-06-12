// Shoot THem Up Game. All Rights Reserved.


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STURespawnComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUPerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);
    bWantsPlayerState = true;
    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUAIController::BeginPlay()
{
    Super::BeginPlay();
    
}

void ASTUAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
    if (STUCharacter)
    {
        if (STUCharacter->BehaviorTreeAsset)
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
        Cast<ASTUAICharacter>(GetPawn())->HealthComponent->OnDamaged.AddUObject(this, &ASTUAIController::OnDamaged);
        GetBlackboardComponent()->ClearValue(LastEnemyPositionKey);
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
void ASTUAIController::OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser)
{
    if (DamageCauser)
    {
        GetBlackboardComponent()->SetValueAsVector(LastEnemyPositionKey, DamageCauser->GetActorLocation());
    }
    
}
