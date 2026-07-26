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
};