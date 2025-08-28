// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "STUCoreTypes.h"

bool USTUGameOverWidget::Initialize()
{
    if (GetWorld())
    {
        const auto Gamemode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (Gamemode)
        {
            Gamemode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }
    return Super::Initialize();
}

void USTUGameOverWidget::UpdatePlayersStat()
{
    if (!GetWorld() || !PlayerStatBox)
        return;
    PlayerStatBox->ClearChildren();
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller)
            continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            continue;
        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget)
            continue;
        PlayerStatBox->AddChild(PlayerStatRowWidget);
        FStatRowInfo RowInfo = {PlayerState->GetPlayerName(), FString::FromInt(PlayerState->GetKillsNum()),
                                FString::FromInt(PlayerState->GetDeathsNum()),
                                FString::FromInt(PlayerState->GetTeamID()), Controller->IsPlayerController()};
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