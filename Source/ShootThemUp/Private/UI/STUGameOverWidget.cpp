// Shoot THem Up Game. All Rights Reserved.

#include "UI/STUGameOverWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STUPlayerState.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.h"
#include "STUGameStateBase.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "STUUtils.h"
#include "Components/TextBlock.h"
void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (GetWorld())
    {
        const auto GameState = Cast<ASTUGameStateBase>(GetWorld()->GetGameState());
        if (GameState)
        {
            GameState->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }
    if (ResetLevelButton)
    {
        if (GetOwningPlayer()->GetLocalRole() != ROLE_Authority)
            ResetLevelButton->SetVisibility(ESlateVisibility::Hidden);
        else
            ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
    }
}

void USTUGameOverWidget::UpdatePlayersStat()
{
    const auto GameState = Cast<ASTUGameStateBase>(GetWorld()->GetGameState());
    if (!GameState)
        return;

    PlayerStatBox->ClearChildren();
    int32 HighestScore = 0;
    FTeamInfo* WinnerTeam = nullptr;
    TArray<FPlayerInfo> MyPlayersInfo;

    for (FTeamInfo &Info : GameState->TeamsStats)
    {
        UE_LOG(LogTemp, Display, TEXT("USTUGameOverWidget::UpdatePlayersStat: Team %d score is %d"), Info.TeamID,
               Info.Score);
        if (Info.Score > HighestScore)
        {
            WinnerTeam = &Info;
        }
        for (FPlayerInfo& PlayerInfo : Info.PlayersInfos)
        {
            MyPlayersInfo.Add(PlayerInfo);
            
        }
    }
    MyPlayersInfo.Sort([](const FPlayerInfo &A, const FPlayerInfo &B) { return A.Kills > B.Kills; });
    for (FPlayerInfo& PlayerInfo : MyPlayersInfo)
    {
        FStatRowInfo RowInfo = STUUtils::GetStatRowInfoFromFPlayerStats(PlayerInfo);

        if (GetWorld()->GetFirstPlayerController()->PlayerState->GetUniqueId()->ToString() == PlayerInfo.PlayerID)
            RowInfo.PlayerIndicatorVisibility = true;
        else
            RowInfo.PlayerIndicatorVisibility = false;

        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget)
            continue;
        PlayerStatBox->AddChild(PlayerStatRowWidget);
        PlayerStatRowWidget->SetRowInfo(RowInfo);
    }
    FString WinnerName = "";
    if (WinnerTeam)
    {
        WinnerName = WinnerTeam->TeamName + " WON";
        
    }
    else
    {
        WinnerName = "DRAW";
    }
    TeamWinText->SetText(FText::FromString(WinnerName));
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver)
    {
        UpdatePlayersStat();
    }
}

void USTUGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}