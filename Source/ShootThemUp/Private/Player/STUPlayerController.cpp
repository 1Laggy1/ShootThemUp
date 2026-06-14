// Shoot THem Up Game. All Rights Reserved.

#include "Player/STUPlayerController.h"
#include "Camera/CameraActor.h"
#include "Components/STURespawnComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpectatorPawn.h"
#include "Net/UnrealNetwork.h"
#include "Player/STUBaseCharacter.h"
#include "STUGameModeBase.h"
#include "STUGameStateBase.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/Lobby/STULobbyGameState.h"
#include "Player/STUPlayerCharacter.h"
#include "Player/STUPlayerState.h"
#include "STUGameStateBase.h"

DECLARE_LOG_CATEGORY_CLASS(LogSTUPlayerController, All, All);

ASTUPlayerController::ASTUPlayerController()
{
    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUPlayerController::OnRequestPossess_Client_Implementation(APawn *InPawn)
{
    if (InPawn == nullptr)
        return;
    bShowMouseCursor = false;
    // ControlledPawn = InPawn;
    OnNewPawnEvent.Broadcast(InPawn);
    SetInputMode(FInputModeGameOnly());
    const auto CharacterPawn = Cast<ASTUBaseCharacter>(InPawn);
    if (CharacterPawn)
    {
        if (CharacterPawn->AbilityClass)
        {
            const auto Compon = CharacterPawn->GetComponentByClass(CharacterPawn->AbilityClass);
            if (Compon && IsLocalPlayerController())
            {
                CharacterPawn->InputComponent->BindAction("Ability", EInputEvent::IE_Pressed,
                                                          Cast<USTUPlayerAbilityUseComponent>(Compon),
                                                          &USTUPlayerAbilityUseComponent::StartUseAbility_Server);
                CharacterPawn->InputComponent->BindAction("Ability", EInputEvent::IE_Released,
                                                          Cast<USTUPlayerAbilityUseComponent>(Compon),
                                                          &USTUPlayerAbilityUseComponent::StopUseAbility_Server);
            }
        }
    }
    UE_LOG(
        LogSTUPlayerController, Display,
        TEXT("ASTUPlayerController::OnRequestPossess_Client_Implementation Possesing character. AbilityClass was %s"),
        *CharacterPawn->AbilityClass->GetFName().ToString());
    if (InPawn->IsA<ASpectatorPawn>())
    {
        //
    }
    else if (InPawn->IsA<ACharacter>())
    {
        //
    }
}

void ASTUPlayerController::RequestPossess_Server_Implementation(APawn *InPawn)
{
    bShowMouseCursor = false;
    Possess(InPawn);
    SetInputMode(FInputModeGameOnly());
    OnRequestPossess_Client(InPawn);
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
    // DOREPLIFETIME(ASTUPlayerController, ControlledPawn);
    DOREPLIFETIME(ASTUPlayerController, LobbyCamera);
}

void ASTUPlayerController::TogglePauseMenu()
{
    if (!PauseMenuWidgetClass)
        return;

    if (!PauseMenuWidget)
    {
        PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
    }

    if (!PauseMenuWidget->IsInViewport())
    {
        PauseMenuWidget->AddToViewport(99);

        bShowMouseCursor = true;
        MovementEnabled = false;
    }
    else
    {
        PauseMenuWidget->RemoveFromParent();

        bShowMouseCursor = false;
        MovementEnabled = true;
    }
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogPlayerController, Display, TEXT("ASTUPlayerController::BeginPlay"));
    if (GetWorld())
    {
        bShowMouseCursor = true;
        const auto Gamemode = Cast<ASTUGameStateBase>(GetWorld()->GetGameState());
        if (Gamemode)
        {
            Gamemode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
        }
        STULobbyGameState = Cast<ASTULobbyGameState>(GetWorld()->GetGameState<ASTULobbyGameState>());
        if (STULobbyGameState)
        {
            TArray<AActor *> Cameras;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
            if (Cameras.IsEmpty() || !Cameras[0])
                return;

            for (AActor *Camera : Cameras)
            {
                if (Camera->Tags.Contains("LobbyCamera"))
                {
                    UE_LOG(LogSTUPlayerController, Warning, TEXT("Lobby Camera pos: %s"),
                           *Camera->GetActorLocation().ToString());
                    if (IsLocalController())
                    {

                        SetViewTarget(Cameras[0]);
                    }
                    break;
                }
            }
        }
    }
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent)
        return;

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::TogglePauseMenu);
}

void ASTUPlayerController::NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest)
{
    Super::NotifyLoadedWorld(WorldPackageName, bFinalDest);
    if (WorldPackageName != "LobbyLevel" && IsLocalController() && bFinalDest)
    {
        UE_LOG(LogTemp, Warning, TEXT("I am Player %s has loaded world, notifying server"), *GetNameSafe(this));
        PlayerLoadedWorld();
    }
}

void ASTUPlayerController::CheckPlayerFullyLoadedWorld()
{
    auto Gamestate = GetWorld()->GetGameState<ASTUGameStateBase>();
    if (Gamestate)
    {
        UE_LOG(LogTemp, Warning, TEXT("I am Player %s has fully loaded world, notifying server"), *GetNameSafe(this));
        GetWorld()->GetTimerManager().ClearTimer(CheckWorldTimerHandle);
        PlayerLoadedWorld();

        return;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("I am Player %s not loaded fully yet, waiting..."), *GetNameSafe(this));
    }
}

void ASTUPlayerController::RequestColorChange_Server_Implementation(const FLinearColor &Color)
{
    if (STULobbyGameState)
    {
        STULobbyGameState->ChangeTeamColor_Server(PlayerState->GetUniqueId()->ToString(), Color);
    }
}

void ASTUPlayerController::RequestTeamNameChange_Server_Implementation(const FString &NewName)
{
    if (STULobbyGameState)
    {
        STULobbyGameState->ChangeTeamName_Server(PlayerState->GetUniqueId()->ToString(), NewName);
    }
}

void ASTUPlayerController::RequestWeaponsChange_Server_Implementation(TSubclassOf<ASTUBaseWeapon> WeaponToChoose)
{
    if (STULobbyGameState)
    {
        STULobbyGameState->ChangeWeapons_Server(WeaponToChoose, PlayerState->GetUniqueId()->ToString());
    }
}

void ASTUPlayerController::RequestAbilityChange_Server_Implementation(
    TSubclassOf<USTUPlayerAbilityUseComponent> AbilityToChoose)
{
    if (STULobbyGameState)
    {
        STULobbyGameState->ChangeAbility_Server(AbilityToChoose, PlayerState->GetUniqueId()->ToString());
    }
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

    if (!LobbyCamera)
        return;

    UE_LOG(LogTemp, Warning, TEXT("Switching view to camera: %s"), *GetNameSafe(LobbyCamera));
    SetViewTarget(LobbyCamera);
}

void ASTUPlayerController::PlayerLoadedWorld_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Player %s loaded world, getting gamemode"), *GetNameSafe(this));
    auto GamemodeBase = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GamemodeBase)
        return;
    UE_LOG(LogTemp, Warning, TEXT("Player %s loaded world, informing gamemode"), *GetNameSafe(this));
    if (Cast<ASTUPlayerState>(PlayerState)->LoadedAndNotifiedServer)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player %s already informed gamemode"), *GetNameSafe(this));
        return;
    }
    GamemodeBase->PlayerConnected(this);
    Cast<ASTUPlayerState>(PlayerState)->LoadedAndNotifiedServer = true;
}
