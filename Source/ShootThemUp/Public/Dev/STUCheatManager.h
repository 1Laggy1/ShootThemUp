#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "STUCheatManager.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
    UFUNCTION(Exec)
    void SpawnDummy();
    UFUNCTION(Exec)
    void Kill();
    UFUNCTION(Exec)
    void PrintTeamsStats();
    UFUNCTION(Exec)
    void PrintTeams();
};