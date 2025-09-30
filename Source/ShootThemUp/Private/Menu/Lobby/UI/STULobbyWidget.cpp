// Shoot THem Up Game. All Rights Reserved.

#include "Menu/Lobby/UI/STULobbyWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/Lobby/STULobbyGameState.h"
#include "Menu/UI/STULevelItemWidget.h"
#include "Menu/UI/STUWeaponItemWidget.h"
#include "STUGameInstance.h"
#include "STUUtils.h"
#include "Weapon/STUBaseWeapon.h"
#include "Player/STUPlayerController.h"
#include "Menu/UI/STUAbilityItemWidget.h"
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

    if (ChangeColorButton)
    {
        ChangeColorButton->OnClicked.AddDynamic(this, &USTULobbyWidget::OnChangeColorClicked);
    }

    if (TeamNameInputBox)
    {
        TeamNameInputBox->OnTextCommitted.AddDynamic(this, &USTULobbyWidget::HandleTeamNameCommitted);
    }
    InitLevelItems();
    InitWeaponsItems();
    InitAbilityItems();
}

void USTULobbyWidget::OnChangeColorClicked()
{
    if (!Slider_R || !Slider_G || !Slider_B)
        return;

    float R = Slider_R->GetValue();
    float G = Slider_G->GetValue();
    float B = Slider_B->GetValue();

    FLinearColor NewColor(R, G, B, 1.0f);
    auto PC = Cast<ASTUPlayerController>(GetWorld()->GetFirstPlayerController());
    if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
        return;
    PC->RequestColorChange_Server(NewColor);
}

void USTULobbyWidget::HandleTeamNameCommitted(const FText &Text, ETextCommit::Type CommitMethod)
{
    FString EnteredText = TeamNameInputBox->GetText().ToString();
    auto PC = Cast<ASTUPlayerController>(GetWorld()->GetFirstPlayerController());
    if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
        return;
    PC->RequestTeamNameChange_Server(EnteredText);
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

void USTULobbyWidget::InitWeaponsItems()
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
        return;
    checkf(STUGameInstance->GeWeaponsData().Num() != 0, TEXT("Levels data cannot be empty"));

    if (!WeaponsItemsBox)
        return;
    WeaponsItemsBox->ClearChildren();

    for (auto WeaponData : STUGameInstance->GeWeaponsData())
    {
        const auto WeaponItemWidget = CreateWidget<USTUWeaponItemWidget>(GetWorld(), WeaponWidgetClass);
        if (!WeaponItemWidget)
            continue;
        WeaponItemWidget->SetWeaponData(WeaponData);
        WeaponItemWidget->OnWeaponSelected.AddUObject(this, &USTULobbyWidget::OnWeaponSelected);

        WeaponsItemsBox->AddChild(WeaponItemWidget);
        WeaponItemWidgets.Add(WeaponItemWidget);
    }

    OnWeaponSelected(STUGameInstance->GeWeaponsData()[0]);
}

void USTULobbyWidget::InitAbilityItems()
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
        return;
    checkf(STUGameInstance->GetAbilityData().Num() != 0, TEXT("Levels data cannot be empty"));

    if (!AbilityItemsBox)
        return;
    AbilityItemsBox->ClearChildren();

    for (auto AbilityData : STUGameInstance->GetAbilityData())
    {
        const auto AbilityItemWidget = CreateWidget<USTUAbilityItemWidget>(GetWorld(), AbilityWidgetClass);
        if (!AbilityItemWidget)
            continue;
        AbilityItemWidget->SetAbilityData(AbilityData);
        AbilityItemWidget->OnAbilitySelected.AddUObject(this, &USTULobbyWidget::OnAbilitySelected);

        AbilityItemsBox->AddChild(AbilityItemWidget);
        AbilityItemWidgets.Add(AbilityItemWidget);
    }

    OnAbilitySelected(STUGameInstance->GetAbilityData()[0]);
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

void USTULobbyWidget::OnWeaponSelected(const FWeaponItemData &Data)
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
    {
        return;
    }
    
    auto PC = Cast<ASTUPlayerController>(GetWorld()->GetFirstPlayerController());
    if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
        return;
    PC->RequestWeaponsChange_Server(Data.WeaponClass);
    for (auto WeaponItemWidget : WeaponItemWidgets)
    {
        if (WeaponItemWidget)
        {
            const auto IsSelected = Data.WeaponClass == WeaponItemWidget->GetWeaponData().WeaponClass;
            WeaponItemWidget->SetSelected(IsSelected);
        }
    }
}

void USTULobbyWidget::OnAbilitySelected(const FAbilityItemData &Data)
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
    {
        return;
    }

    auto PC = Cast<ASTUPlayerController>(GetWorld()->GetFirstPlayerController());
    if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
        return;
    PC->RequestAbilityChange_Server(Data.AbilityClass);
    for (auto AbilityItemWidget : AbilityItemWidgets)
    {
        if (AbilityItemWidget)
        {
            const auto IsSelected = Data.AbilityClass == AbilityItemWidget->GetAbilityData().AbilityClass;
            AbilityItemWidget->SetSelected(IsSelected);
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

    // for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
    //{
    //     APlayerController *PC = Iterator->Get();
    //     if (PC && !PC->IsLocalController())
    //     {
    //         PC->Destroy();
    //     }
    // }

    // ServerTravel

    FString Level = "/Game/Levels/" + GetSTUGameInstance()->GetStartupLevel().LevelName.ToString();
    World->ServerTravel(Level, true, false);
}
void USTULobbyWidget::OnQuitGame()
{
    // GetSTUGameInstance()->CloseSession();
    UGameplayStatics::OpenLevel(this, GetSTUGameInstance()->GetMainMenuLevelName());
}