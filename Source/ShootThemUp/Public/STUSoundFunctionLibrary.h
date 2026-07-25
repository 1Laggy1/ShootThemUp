// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "STUSoundFunctionLibrary.generated.h"

class USoundClass;
UCLASS()
class SHOOTTHEMUP_API USTUSoundFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
  public:
    UFUNCTION(BlueprintCallable)
    static void SetSoundClassVolume(USoundClass *SoundClass, float Volume);
    UFUNCTION(BlueprintCallable)
    static void ToggleSoundClassVolume(USoundClass *SoundClass);

	
};
