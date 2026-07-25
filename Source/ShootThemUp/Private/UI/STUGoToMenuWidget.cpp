// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUGoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

void USTUGoToMenuWidget::NativeOnInitialized()
{
    if (GoToMenuButton)
    {
        GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnMainMenu);
    }
}

void USTUGoToMenuWidget::OnMainMenu()
{
    if (!GetWorld() || !GetWorld()->GetGameInstance())
        return;

    UGameplayStatics::OpenLevel(this, GetWorld()->GetGameInstance<USTUGameInstance>()->GetMainMenuLevelName());
}
