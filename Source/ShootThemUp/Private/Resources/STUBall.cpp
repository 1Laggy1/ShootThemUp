// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUBall.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "UObject/ConstructorHelpers.h"

ASTUBall::ASTUBall()
{
	PrimaryActorTick.bCanEverTick = false;
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    PointLight->SetupAttachment(RootComponent);
}

void ASTUBall::BeginPlay()
{
    Super::BeginPlay();

    if (MeshComponent)
    {
        CurrentMaterial = MeshComponent->GetMaterial(0);

        if (CurrentMaterial)
        {
            UE_LOG(LogTemp, Warning, TEXT("Material found: %s"), *CurrentMaterial->GetName());
        }
    }
}