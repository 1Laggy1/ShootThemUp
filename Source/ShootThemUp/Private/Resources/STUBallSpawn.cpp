// Shoot THem Up Game. All Rights Reserved.

#include "Resources/STUBallSpawn.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "STUGameModeBase.h"
#include "UI/STUBallSpawnWidget.h"

ASTUBallSpawn::ASTUBallSpawn()
{
    PrimaryActorTick.bCanEverTick = true;
    USceneComponent *SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneComp;
    SpawnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SpawnWidgetComponent"));
    SpawnWidgetComponent->SetupAttachment(RootComponent);
    bReplicates = true;
}

void ASTUBallSpawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{

    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // DOREPLIFETIME(ASTUPlayerController, ControlledPawn);
    DOREPLIFETIME(ASTUBallSpawn, TimeLeftToSpawn);
    DOREPLIFETIME(ASTUBallSpawn, TimeToSpawnStart);
}
void ASTUBallSpawn::BeginPlay()
{
    Super::BeginPlay();

    if (SpawnWidgetComponent)
    {

        SpawnWidget = Cast<USTUBallSpawnWidget>(SpawnWidgetComponent->GetUserWidgetObject());
        if (SpawnWidget)
        {
            SpawnWidget->BallSpawnInstance = this;
        }
    }
}
void ASTUBallSpawn::StartSpawning(float TimeToSpawn)
{
    if (TimeToSpawnStart > 0 || !HasAuthority())
        return;
    TimeToSpawnStart = TimeToSpawn;
    TimeLeftToSpawn = TimeToSpawn;
    SetVisible(true);
}

void ASTUBallSpawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (TimeToSpawnStart > 0)
    {
        if (HasAuthority())
        {
            if (TimeLeftToSpawn <= 0)
            {
                TimeToSpawnStart = 0;
                SpawnBall();
                SetVisible(false);
            }
            TimeLeftToSpawn -= DeltaTime;
        }
    }
}

void ASTUBallSpawn::SpawnBall()
{
    AActor *Ball = GetWorld()->SpawnActor<ASTUBall>(BallClass, GetActorTransform());
    const auto STUGamemode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
    if (STUGamemode && Ball)
    {
        STUGamemode->SetBallInstance(Ball);
    }
}

void ASTUBallSpawn::SetVisible_Implementation(bool Visible)
{
    ESlateVisibility IsVisible = Visible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
    SpawnWidget->SetVisibility(IsVisible);
}