// Shoot THem Up Game. All Rights Reserved.


#include "Menu/Lobby/UI/STULobbyWidget.h"
#include "Menu/UI/STULevelItemWidget.h"
#include "Components/ScrollBox.h"
#include "STUGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "STUUtils.h"
void USTULobbyWidget::NativeOnInitialized()
{
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &USTULobbyWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &USTULobbyWidget::OnQuitGame);
    }
    InitLevelItems();
}

void USTULobbyWidget::InitLevelItems()
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
        return;
    checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data cannot be empty"));

    if (!LevelItemsBox)
        return;
    LevelItemsBox->ClearChildren();

    for (auto LevelData : STUGameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget)
            continue;
        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USTULobbyWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    if (STUGameInstance->GetStartupLevel().LevelName.IsNone())
    {
        OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
    }
    else
    {
        OnLevelSelected(STUGameInstance->GetStartupLevel());
    }
}

void USTULobbyWidget::OnLevelSelected(const FLevelData &Data)
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
    {
        return;
    }

    STUGameInstance->SetStartupLevel(Data);
    for (auto LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

USTUGameInstance *USTULobbyWidget::GetSTUGameInstance() const
{
    if (!GetWorld())
        return nullptr;
    return GetWorld()->GetGameInstance<USTUGameInstance>();
}

void USTULobbyWidget::OnStartGame()
{

    UWorld *World = GetWorld();
    if (!World)
        return;

    AGameModeBase *GM = World->GetAuthGameMode();
    if (!GM)
        return;

    //for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
    //{
    //    APlayerController *PC = Iterator->Get();
    //    if (PC && !PC->IsLocalController())
    //    {
    //        PC->Destroy();
    //    }
    //}
    
    // ServerTravel

    FString Level = "/Game/Levels/" + GetSTUGameInstance()->GetStartupLevel().LevelName.ToString();
    World->ServerTravel(Level, true, false);
}
void USTULobbyWidget::OnQuitGame()
{
   // GetSTUGameInstance()->CloseSession();
    UGameplayStatics::OpenLevel(this, GetSTUGameInstance()->GetMainMenuLevelName());
}