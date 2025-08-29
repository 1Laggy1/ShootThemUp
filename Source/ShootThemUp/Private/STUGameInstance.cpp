// Shoot THem Up Game. All Rights Reserved.


#include "STUGameInstance.h"
#include "STUSoundFunctionLibrary.h"

void USTUGameInstance::ToggleVolume()
{
    USTUSoundFunctionLibrary::ToggleSoundClassVolume(MasterSoundClass);
}
