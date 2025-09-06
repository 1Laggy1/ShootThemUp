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

    for (auto Info : GameState->MatchStatistics.Stats)
    {

        FPlayerStats PlayerStat = Info;
        
        FStatRowInfo RowInfo = STUUtils::GetStatRowInfoFromFPlayerStats(PlayerStat);
        
        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget)
            continue;
        PlayerStatBox->AddChild(PlayerStatRowWidget);
        PlayerStatRowWidget->SetRowInfo(RowInfo);
    }
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