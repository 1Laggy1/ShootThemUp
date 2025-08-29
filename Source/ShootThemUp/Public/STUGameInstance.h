// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

class USoundClass;

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()
  public:
    FLevelData GetStartupLevel()
    {
        return StartupLevel;
    }
    void SetStartupLevel(const FLevelData &Data)
    {
        StartupLevel = Data;
    }
    TArray<FLevelData> GetLevelsData() const
    {
        return LevelsData;
    }
    FName GetMainMenuLevelName()
    {
        return MainMenuLevelName;
    }
    void ToggleVolume();
  protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FLevelData> LevelsData;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName StartupLevelName = NAME_None;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MainMenuLevelName = NAME_None;
    private:
    FLevelData StartupLevel;
      UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundClass *MasterSoundClass;
};
