// Shoot THem Up Game. All Rights Reserved.


#include "Menu/Lobby/UI/STULobbyHUD.h"
#include "UI/STUBaseWidget.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"

void ASTULobbyHUD::BeginPlay()
{
    Super::BeginPlay();
    if (LobbyWidgetClass)
    {
        const auto LobbyWidget = CreateWidget<USTUBaseWidget>(GetWorld(), LobbyWidgetClass);
        if (LobbyWidget)
        {
            LobbyWidget->AddToViewport();
            LobbyWidget->Show();
        }
    }
}