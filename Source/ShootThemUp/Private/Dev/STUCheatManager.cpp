#include "Dev/STUCheatManager.h"
#include "GameFramework/DamageType.h"
#include "STUGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/DamageEvents.h"
#include "STUUtils.h"
#include "STUGameStateBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCheatManager, All, All)
void USTUCheatManager::PrintTeams()
{
    USTUGameInstance* STUGameInstance = Cast<USTUGameInstance>(GetWorld()->GetGameInstance());
    if (STUGameInstance)
    {
        STUUtils::DebugPrintAllTeams(STUGameInstance->GetTeams());
    }
}

void USTUCheatManager::PrintTeamsStats()
{
    ASTUGameStateBase* STUGameState = GetWorld()->GetGameState<ASTUGameStateBase>();
    if (STUGameState)
    {
        STUUtils::DebugPrintAllTeams(STUGameState->GetTeams());
    }
}

void USTUCheatManager::SpawnDummy()
{
    if (!GetWorld())
        return;

    ASTUGameModeBase *GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->SpawnDummy(1);
        UE_LOG(LogCheatManager, Log, TEXT("CheatManager: Spawned dummy bot via console command!"));
    }
    else
    {
        UE_LOG(LogCheatManager, Warning, TEXT("CheatManager: GameMode not found!"));
    }
}

void USTUCheatManager::Kill()
{
    if (APlayerController* PC = GetOuterAPlayerController())
    {
        
        if (APawn* PlayerPawn = PC->GetPawn())
        {
            FPointDamageEvent DEvent;
                    DEvent.DamageTypeClass = UDamageType::StaticClass();
            PlayerPawn->TakeDamage(1000, DEvent, PC, PlayerPawn);
            
        }
    }
}