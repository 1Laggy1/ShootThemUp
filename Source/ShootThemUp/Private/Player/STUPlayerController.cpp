// Shoot THem Up Game. All Rights Reserved.

#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpectatorPawn.h"
#include "STUGameModeBase.h"
#include "STUGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Player/STUBaseCharacter.h"
#include "Camera/CameraActor.h"

#include "Kismet/GameplayStatics.h"

ASTUPlayerController::ASTUPlayerController()
{
    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUPlayerController::StartSpectatingMulticast_Implementation(APawn *PawnSpectator)
{
    UnPossess();
    Possess(PawnSpectator);
    SetViewTargetWithBlend(PawnSpectator, 0.0f);
    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
}
void ASTUPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{

    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTUPlayerController, ControlledPawn);
}

void ASTUPlayerController::OnPossess(APawn *InPawn)
{
    if (InPawn == nullptr)
        return;
    ControlledPawn = InPawn;
    Super::OnPossess(InPawn);
    OnNewPawnEvent.Broadcast(InPawn);
    if (InPawn->IsA<ASpectatorPawn>())
    {
        //
    }
    else if (InPawn->IsA<ACharacter>())
    {
        //
    }
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (GetWorld())
    {
        bShowMouseCursor = true;
        const auto Gamemode = Cast<ASTUGameStateBase>(GetWorld()->GetGameState());
        if (Gamemode)
        {
            Gamemode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
        }
    }
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent)
        return;

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
}

void ASTUPlayerController::OnPauseGame()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode())
        return;

    // GetWorld()->GetAuthGameMode()->SetPause(this, FCanUnpause());
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void ASTUPlayerController::SetCamera_Implementation(FVector Location, FRotator Rotation)
{

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    ACameraActor *TempCamera = GetWorld()->SpawnActor<ACameraActor>(Location, Rotation, SpawnParams);
    SetViewTarget(TempCamera);

    TArray<AActor *> Cameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
    for (AActor *Camera : Cameras)
    {
        if (Camera != TempCamera)
            Camera->Destroy();
    }
}
