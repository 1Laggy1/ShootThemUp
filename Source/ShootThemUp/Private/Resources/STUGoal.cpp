// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUGoal.h"
#include "Components/BoxComponent.h"
#include "Resources/STUBall.h"
#include "NiagaraFunctionLibrary.h"
#include "STUGameModeBase.h"

ASTUGoal::ASTUGoal()
{
	PrimaryActorTick.bCanEverTick = false;
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    BoxComponent->SetGenerateOverlapEvents(true);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
    BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    bReplicates = true; 

}

void ASTUGoal::BeginPlay()
{
	Super::BeginPlay();
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUGoal::OnBoxOverlapBegin);
    STUGameModeBase = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
	
}

void ASTUGoal::OnBoxOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                                 UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult &SweepResult)
{
    if (!HasAuthority())
        return;
    if (!OtherActor || OtherActor == this)
        return;
    const auto Ball = Cast<ASTUBall>(OtherActor);
    if (!Ball)
        return;
    Goal(Ball);
}

void ASTUGoal::Goal(ASTUBall *Ball)
{
    GoalFX_Multicast();
    STUGameModeBase->Goal(Ball, this);
}

void ASTUGoal::GoalFX_Multicast_Implementation()
{
     UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GoalNiagaraEffect, GetActorLocation(),
                                                   GetActorRotation());
}

