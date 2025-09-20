// Shoot THem Up Game. All Rights Reserved.

#include "STUGameStateBase.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerState.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.h"
#include "STUUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
void ASTUGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate current health.
    DOREPLIFETIME(ASTUGameStateBase, GameData);
    // DOREPLIFETIME(ASTUGameStateBase, CurrentRound);
    DOREPLIFETIME(ASTUGameStateBase, GameCountDown);
    DOREPLIFETIME(ASTUGameStateBase, MatchState);
    DOREPLIFETIME(ASTUGameStateBase, WaitingTimeNow);
    DOREPLIFETIME(ASTUGameStateBase, TeamsStats);
    
    // DOREPLIFETIME(ASTUGameStateBase, MatchStatistics);
}


void ASTUGameStateBase::BeginPlay()
{
    Super::BeginPlay();
    SetReplicates(true);
    bReplicates = true;
    PlayerConnected(GetWorld()->GetFirstPlayerController());
}

void ASTUGameStateBase::PlayerConnected_Implementation(APlayerController *PC)
{
    GetWorld()->GetAuthGameMode<ASTUGameModeBase>()->PlayerConnected(PC);
}

// void ASTUGameStateBase::OnRep_MatchStateChanged()
//{
//     OnMatchStateChanged.Broadcast(MatchState);
// }
void ASTUGameStateBase::ResetOnePlayerMulticast_Implementation(AActor *DiedActor, AActor *Spawn)
{
    /*if (DiedActor && Cast<ACharacter>(DiedActor) && Cast<ACharacter>(DiedActor)->Controller)
    {

        Cast<ACharacter>(DiedActor)->Reset();
        DiedActor->SetActorLocation(Spawn->GetActorLocation());
        DiedActor->SetActorRotation(Spawn->GetActorRotation());
    }*/
}
// void ASTUGameStateBase::SetPlayerColorMulticast_Implementation(AActor *Player, FLinearColor TeamColor)
//{
//     if (!Player)
//         return;
//
//     const auto Character = Cast<ASTUBaseCharacter>(Player);
//     if (!Character)
//         return;
//
//     Character->SetPlayerColor(TeamColor);
// }
//
// void ASTUGameStateBase::InitPlayer_Multicast_Implementation(const FString &PlayerID, ASTUBaseCharacter *Character)
//{
//
//     FTimerHandle TempHandle;
//     GetWorld()->GetTimerManager().SetTimer(
//         TempHandle,
//         [this, Character, PlayerID, TempHandle]() mutable {
//             if (!Character || !GetWorld() || !IsValid(Character) || !Character->HasActorBegunPlay() ||
//             !Character->GetWorld())
//                 InitPlayer_Multicast_Implementation(PlayerID, Character);
//
//             auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
//             if (!STUGameInstance)
//                 InitPlayer_Multicast_Implementation(PlayerID, Character);
//
//             FPlayerInfo *PlayerInfo = STUUtils::FindPlayerByPlayerID(PlayerID, STUGameInstance->Teams);
//             if (!PlayerInfo)
//                 InitPlayer_Multicast_Implementation(PlayerID, Character);
//
//             //Character->InitPlayer(PlayerID, GetWorld());
//             Character->SetPlayerColor(PlayerInfo->Color);
//
//             GetWorld()->GetTimerManager().ClearTimer(TempHandle);
//         },
//         0.5f, false);
//
//
// }