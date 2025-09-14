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

DECLARE_LOG_CATEGORY_CLASS(LogSTUPlayerController, All, All);

ASTUPlayerController::ASTUPlayerController()
{
    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUPlayerController::Possess_Client_Implementation(APawn *InPawn)
{
    if (InPawn == nullptr)
    {
        UE_LOG(LogSTUPlayerController, Warning, TEXT("Possess_Client: InPawn was nullptr"));
        return;
    }
    Possess(InPawn);
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
    DOREPLIFETIME(ASTUPlayerController, LobbyCamera);
}

void ASTUPlayerController::OnRep_Possesed()
{
    
    
    FTimerHandle TempHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TempHandle,
        [this]() {
            if (ControlledPawn)
            {
                Possess(ControlledPawn);
                OnNewPawnEvent.Broadcast(ControlledPawn);
                return;
            }
            OnRep_Possesed();
        },
        0.5f, false);
}

void ASTUPlayerController::OnPossess(APawn *InPawn)
{
    bShowMouseCursor = false;
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

void ASTUPlayerController::OnRep_SetCamera()
{
    /*FTimerHandle TempHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TempHandle,
        [this]() {
            if (LobbyCamera)
            {
                SetViewTarget(LobbyCamera); 
                return;
            }
            OnRep_SetCamera();
        },
        0.5f, false);*/
    if (!LobbyCamera)
        return;

    UE_LOG(LogTemp, Warning, TEXT("Switching view to camera: %s"), *GetNameSafe(LobbyCamera));
    SetViewTarget(LobbyCamera);
}
