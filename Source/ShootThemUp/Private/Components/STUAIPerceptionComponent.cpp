// Shoot THem Up Game. All Rights Reserved.


#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthActorComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "AI/STUAICharacter.h"


AActor *USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor *> SightPercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SightPercieveActors);
    TArray<AActor *> DamagePercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), DamagePercieveActors);
    TArray<AActor *> PercieveActors;
    for (AActor* ActorToAdd : SightPercieveActors)
    {
        PercieveActors.AddUnique(ActorToAdd);
    }
    for (AActor *ActorToAdd : DamagePercieveActors)
    {
        PercieveActors.AddUnique(ActorToAdd);
    }
    
    if (PercieveActors.Num() == 0)
        return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller)
        return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
        return nullptr;

    float BestDistance = MAX_FLT;
    AActor *BestPawn = nullptr;
    for (const auto PercieveActor : PercieveActors)
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(PercieveActor);
        const auto PercievePawn = Cast<APawn>(PercieveActor);
        const auto AreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->Controller);
        if (HealthComponent && !HealthComponent->isDead() && AreEnemies)
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }
    return BestPawn;
}

AActor *USTUAIPerceptionComponent::GetDamageEnemy() const
{
    TArray<AActor *> DamagePercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), DamagePercieveActors);
    if (!DamagePercieveActors.IsEmpty())
    {
        return DamagePercieveActors[0];
    }
    return nullptr;
}
