// Shoot THem Up Game. All Rights Reserved.


#include "Menu/Lobby/UI/STULobbyHUD.h"
#include "UI/STUBaseWidget.h"

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