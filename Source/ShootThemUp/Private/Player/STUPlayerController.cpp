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
#include "STUGameStateBase.h"
#include "Player/STUPlayerState.h"
#include "Menu/Lobby/STULobbyGameState.h"

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

//void ASTUPlayerController::Possess_Client_Implementation(APawn *InPawn)
//{
//    if (InPawn == nullptr)
//    {
//        UE_LOG(LogSTUPlayerController, Warning, TEXT("Possess_Client: InPawn was nullptr"));
//        return;
//    }
//    UE_LOG(LogSTUPlayerController, Display, TEXT("Possess_Client: Possesing %s"), *InPawn->GetFullName());
//    Possess(InPawn);
//}

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
    //DOREPLIFETIME(ASTUPlayerController, ControlledPawn);
    DOREPLIFETIME(ASTUPlayerController, LobbyCamera);
}

//void ASTUPlayerController::OnRep_Possesed()
//{
//    
//    
//    FTimerHandle TempHandle;
//    GetWorld()->GetTimerManager().SetTimer(
//        TempHandle,
//        [this]() {
//            if (ControlledPawn)
//            {
//                Possess(ControlledPawn);
//                //OnNewPawnEvent.Broadcast(ControlledPawn);
//                UE_LOG(LogPlayerController, Display, TEXT("%s POSSESING %s"), *GetFullName(),
//                       *ControlledPawn->GetFullName());
//                return;
//            }
//            OnRep_Possesed();
//        },
//        0.5f, false);
//}


//void ASTUPlayerController::OnPossess(APawn *InPawn)
//{
//    if (InPawn == nullptr)
//        return;
//    bShowMouseCursor = false;
//    //ControlledPawn = InPawn;
//    Super::OnPossess(InPawn);
//    OnNewPawnEvent.Broadcast(InPawn);
//    if (InPawn->IsA<ASpectatorPawn>())
//    {
//        //
//    }
//    else if (InPawn->IsA<ACharacter>())
//    {
//        //
//    }
//}

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
                if (Camera->Tags.Contains("LobbyCamera")) // or check by name
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

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
}

void ASTUPlayerController::NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest)
{
    Super::NotifyLoadedWorld(WorldPackageName, bFinalDest);
    if (WorldPackageName != "LobbyLevel" && IsLocalController() && bFinalDest)
    {
        UE_LOG(LogTemp, Warning, TEXT("I am Player %s has loaded world, notifying server"), *GetNameSafe(this));
        PlayerLoadedWorld();
        //GetWorld()->GetTimerManager().SetTimer(CheckWorldTimerHandle, this,
        //                                       &ASTUPlayerController::CheckPlayerFullyLoadedWorld,
        //                                      0.5f, true);
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
        STULobbyGameState->ChangeTeamColor_Server(
            PlayerState->GetUniqueId()->ToString(), Color);
    }
}

void ASTUPlayerController::RequestTeamNameChange_Server_Implementation(const FString &NewName)
{
    if (STULobbyGameState)
    {
        STULobbyGameState->ChangeTeamName_Server(
            PlayerState->GetUniqueId()->ToString(), NewName);
    }
}

void ASTUPlayerController::RequestWeaponsChange_Server_Implementation(TSubclassOf<ASTUBaseWeapon> WeaponToChoose)
{
    if (STULobbyGameState)
    {
        STULobbyGameState->ChangeWeapons_Server(
            WeaponToChoose, PlayerState->GetUniqueId()->ToString());
    }
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
