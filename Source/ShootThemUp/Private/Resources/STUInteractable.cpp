// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUInteractable.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

// Sets default values
ASTUInteractable::ASTUInteractable()
{
    PrimaryActorTick.bCanEverTick = true;
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);

}

void ASTUInteractable::NotifyActorBeginOverlap(AActor *OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (!CouldBeTaken())
    {
        return;
    }
    if (!CollisionComponent)
        return;
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ASTUInteractable::Cooldown, CooldownTime);

    Interact(OtherActor);
}

void ASTUInteractable::Interact(AActor *OtherActor)
{
}

void ASTUInteractable::Cooldown()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

bool ASTUInteractable::CouldBeTaken()
{
    return !GetWorldTimerManager().IsTimerActive(CooldownTimerHandle);
}

