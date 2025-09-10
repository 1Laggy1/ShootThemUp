// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STUPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNewPawnEvent, APawn*);
/**
 * 
 */
class USTURespawnComponent;
class ASTUBaseCharacter;
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()
  public:
    
    FOnNewPawnEvent OnNewPawnEvent;
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
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
    UFUNCTION()
    void OnRep_Possesed()
    {
        OnNewPawnEvent.Broadcast(ControlledPawn);
    }
    UFUNCTION(NetMulticast, Reliable)
    void StartSpectatingMulticast(APawn* PawnSpectator);
    UPROPERTY(ReplicatedUsing = OnRep_Possesed)
    APawn *ControlledPawn;
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
