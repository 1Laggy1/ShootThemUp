// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/STUBaseWidget.h"
#include "STUGameModeBase.h"
#include "STUUtils.h"
#include "STUGameStateBase.h"
DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);
void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
    //DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();
    if (!GetOwningPlayerController() || !GetOwningPlayerController()->IsLocalPlayerController())
        return;
    GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget)
            continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        OnMatchStateChanged(ESTUMatchState::InProgress);
        if (auto GS = GetWorld()->GetGameState<ASTUGameStateBase>())
        {
            GS->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
        }
        /*const auto Gamemode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (Gamemode)
        {
            Gamemode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
        }*/
    }

}

void ASTUGameHUD::DrawCrossHair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max-HalfLineSize, Center.Min, Center.Max+HalfLineSize, LineColor, LineThickness);
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    

    if (GameWidgets.Contains(State))
    {
        if (CurrentWidget)
        {
            CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        CurrentWidget = GameWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }
    UE_LOG(LogSTUGameHUD, Display, TEXT("Gamemode changed: %s"), *UEnum::GetValueAsString(State));
}

