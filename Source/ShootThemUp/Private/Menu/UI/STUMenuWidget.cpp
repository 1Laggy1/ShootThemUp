// Shoot THem Up Game. All Rights Reserved.


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
    }
    /*GetSTUGameInstance()->GetOnlineSubsystem()->GetSessionInterface()->OnCreateSessionCompleteDelegates.AddUObject(
        this, &USTUMenuWidget::OnCreateSessionComplete);*/
}

//void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation *Animation)
//{
//    /*if (Animation == LoadAnim)
//    {
//        const auto STUGameInstance = GetSTUGameInstance();
//        if (!STUGameInstance)
//            return;
//
//        UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
//    }
//    */
//}
//
//void USTUMenuWidget::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
//{
//    if (bWasSuccessful)
//    {
//        UE_LOG(LogSTUMenuWidget, Display, TEXT("Session created successfully"));
//
//        // Simple server travel - let Unreal handle the net driver
//        FString Level = "/Game/Levels/" + GetSTUGameInstance()->GetLobbyLevelName().ToString() + "?listen";
//        GetWorld()->ServerTravel(Level);
//    }
//    else
//        StopAnimation(LoadAnim);
//}
void USTUMenuWidget::OnStartGame()
{
    PlayAnimation(LoadAnim);
    
    
    Cast<USTUGameInstance>(GetGameInstance())->CreateLobby();
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

USTUGameInstance *USTUMenuWidget::GetSTUGameInstance() const
{
    if (!GetWorld())
        return nullptr;
    return GetWorld()->GetGameInstance<USTUGameInstance>();
}
