// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STUPlayerController.generated.h"


/**
 * 
 */
class USTURespawnComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()
  public:
    ASTUPlayerController();
    void SetPreviousCameraPosition(FVector PreviousLocation, FRotator PreviousRotation)
    {
        PreviousCameraPosition = PreviousLocation;
        PreviousCameraRotation = PreviousRotation;
    };
    FVector GetPreviousCameraPosition()
    {
        return PreviousCameraPosition;
    };
    FRotator GetPreviousCameraRotation()
    {
        return PreviousCameraRotation;
    };
    
    UFUNCTION(NetMulticast, Reliable)
    void StartSpectatingMulticast(APawn* PawnSpectator);
  protected:
    
    
    virtual void OnPossess(APawn *InPawn) override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USTURespawnComponent *STURespawnComponent;
    FVector PreviousCameraPosition;
    FRotator PreviousCameraRotation;
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
  private:
    void OnPauseGame();
    void OnMatchStateChanged(ESTUMatchState State);
    

};
