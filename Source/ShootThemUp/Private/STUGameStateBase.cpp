// Shoot THem Up Game. All Rights Reserved.


#include "STUGameStateBase.h"
#include "GameFramework/Character.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "STUGameModeBase.h"
void ASTUGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate current health.
    DOREPLIFETIME(ASTUGameStateBase, GameData);
    DOREPLIFETIME(ASTUGameStateBase, CurrentRound);
    DOREPLIFETIME(ASTUGameStateBase, RoundCountDown);
    DOREPLIFETIME(ASTUGameStateBase, MatchState);
    //DOREPLIFETIME(ASTUGameStateBase, MatchStatistics);
}

void ASTUGameStateBase::BeginPlay()
{
    Super::BeginPlay();
    SetReplicates(true);
    bReplicates = true;
}

//void ASTUGameStateBase::OnRep_MatchStateChanged()
//{
//    OnMatchStateChanged.Broadcast(MatchState);
//}
void ASTUGameStateBase::ResetOnePlayerMulticast_Implementation(AActor *DiedActor, AActor *Spawn)
{
    /*if (DiedActor && Cast<ACharacter>(DiedActor) && Cast<ACharacter>(DiedActor)->Controller)
    {

        Cast<ACharacter>(DiedActor)->Reset();
        DiedActor->SetActorLocation(Spawn->GetActorLocation());
        DiedActor->SetActorRotation(Spawn->GetActorRotation());
    }*/
}
void ASTUGameStateBase::SetPlayerColorMulticast_Implementation(AActor *Player, FLinearColor TeamColor)
{
    if (!Player)
        return;

    const auto Character = Cast<ASTUBaseCharacter>(Player);
    if (!Character)
        return;

    Character->SetPlayerColor(TeamColor);
}