// Shoot THem Up Game. All Rights Reserved.

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}