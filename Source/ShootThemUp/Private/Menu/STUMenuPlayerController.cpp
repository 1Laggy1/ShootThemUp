// Shoot THem Up Game. All Rights Reserved.


#include "Menu/STUMenuPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void ASTUMenuPlayerController::BeginPlay()
{
        TArray<AActor*> IntroCameras;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("IntroCamera"), IntroCameras);
        
        if (IntroCameras.Num() > 0)
        {
            int32 RandomIndex = FMath::RandHelper(IntroCameras.Num());  
            AActor* RandomCamera = IntroCameras[RandomIndex];
            SetViewTargetWithBlend(RandomCamera);
        }

    Super::BeginPlay();
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    
}
