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
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Player/STUPlayerController.h"

void ASTUGameStateBase::OnRep_TimerChanged()
{
    int32 TimeNow = WaitingTimeNow + BetweenGoalsCountDown;
    OnTimerChanged.Broadcast(TimeNow);
}
void ASTUGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASTUGameStateBase, GameData);
    // DOREPLIFETIME(ASTUGameStateBase, CurrentRound);
    DOREPLIFETIME(ASTUGameStateBase, GameCountDown);
    DOREPLIFETIME(ASTUGameStateBase, MatchState);
    DOREPLIFETIME(ASTUGameStateBase, WaitingTimeNow);
    DOREPLIFETIME(ASTUGameStateBase, TeamsStats);
    DOREPLIFETIME(ASTUGameStateBase, BetweenGoalsCountDown);
    DOREPLIFETIME(ASTUGameStateBase, AfterGoalCountDown);
    
    // DOREPLIFETIME(ASTUGameStateBase, MatchStatistics);
}


void ASTUGameStateBase::BeginPlay()
{
    Super::BeginPlay();
    SetReplicates(true);
    bReplicates = true;
    GetWorld()->GetTimerManager().SetTimer(WaitForOutlineColorsTimerHandle, this, &ASTUGameStateBase::SetOutlineColors,
                                           1.0f, true);
    //PlayerConnected(GetWorld()->GetFirstPlayerController());
}

void ASTUGameStateBase::ResetOnePlayerMulticast_Implementation(AActor *DiedActor, AActor *Spawn)
{
    /*if (DiedActor && Cast<ACharacter>(DiedActor) && Cast<ACharacter>(DiedActor)->Controller)
    {

        Cast<ACharacter>(DiedActor)->Reset();
        DiedActor->SetActorLocation(Spawn->GetActorLocation());
        DiedActor->SetActorRotation(Spawn->GetActorRotation());
    }*/
}

void ASTUGameStateBase::OnRep_TeamsStats()
{
    //SetOutlineColors();
}

void ASTUGameStateBase::SetOutlineColors()
{

  if (!OutlineColorsMPC || TeamsStats.IsEmpty())
  {
        return;
  }
    
    
    int32 LocalPlayerTeamID = 1;
    ASTUPlayerController *PC =
        Cast<ASTUPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PC && PC->PlayerState) {
      ASTUPlayerState *PS = PC->GetPlayerState<ASTUPlayerState>();
      LocalPlayerTeamID = PS->GetTeamID();
    }

    if (LocalPlayerTeamID == 0)
    {
        return;
    }
      

    int32 EnemyTeamID = (LocalPlayerTeamID == 1) ? 2 : 1;

    FTeamInfo *LocalTeam =
        STUUtils::FindTeamByTeamID(LocalPlayerTeamID, TeamsStats);
    FTeamInfo *EnemyTeam = STUUtils::FindTeamByTeamID(EnemyTeamID, TeamsStats);

    if (LocalTeam && EnemyTeam) {

      UKismetMaterialLibrary::SetVectorParameterValue(
          GetWorld(), OutlineColorsMPC, FName("Team1Color"),
          LocalTeam->TeamColor);

      UKismetMaterialLibrary::SetVectorParameterValue(
          GetWorld(), OutlineColorsMPC, FName("Team2Color"),
          EnemyTeam->TeamColor);
          GetWorld()->GetTimerManager().ClearTimer(WaitForOutlineColorsTimerHandle);
    }
  
}

void ASTUGameStateBase::SetTeams(const TArray<FTeamInfo>& NewTeams)
{
    TeamsStats = NewTeams;
}