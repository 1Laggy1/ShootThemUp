#include "Dev/STUCheatManager.h"
#include "STUGameModeBase.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogCheatManager, All, All)

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