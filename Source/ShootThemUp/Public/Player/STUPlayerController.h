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
class UCameraComponent;
class ASTUGameStateBase;
class ASTULobbyGameState;
class UUserWidget;
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
    GENERATED_BODY()
  public:
    FOnNewPawnEvent OnNewPawnEvent;
    ASTUPlayerController();
    UFUNCTION(Server, Reliable)
    void RequestPossess_Server(APawn *InPawn);
    UFUNCTION(Client, Reliable)
    void OnRequestPossess_Client(APawn *InPawn);
    /*UFUNCTION(Client, Reliable)
    void Possess_Client(APawn *InPawn);*/
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
    /*UFUNCTION()
    void OnRep_Possesed();*/
    UFUNCTION(NetMulticast, Reliable)
    void StartSpectatingMulticast(APawn *PawnSpectator);
    // UPROPERTY(Replicated)//Using = OnRep_Possesed)
    // APawn *ControlledPawn;
    FPlayerInfo SpawnInfo;
    UPROPERTY(ReplicatedUsing = OnRep_SetCamera)
    ACameraActor *LobbyCamera;
    UFUNCTION()
    void OnRep_SetCamera();
    UFUNCTION(Server, Reliable)
    void RequestColorChange_Server(const FLinearColor &Color);
    UFUNCTION(Server, Reliable)
    void RequestTeamNameChange_Server(const FString &NewName);
    UFUNCTION(Server, Reliable)
    void RequestWeaponsChange_Server(TSubclassOf<ASTUBaseWeapon> WeaponToChoose);
    UFUNCTION(Server, Reliable)
    void RequestAbilityChange_Server(TSubclassOf<USTUPlayerAbilityUseComponent> AbilityToChoose);
    UFUNCTION(BlueprintCallable, Category = "UI")
    void TogglePauseMenu();
  protected:
    //virtual void OnPossess(APawn *InPawn) override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USTURespawnComponent *STURespawnComponent;
    FVector PreviousCameraPosition;
    FRotator PreviousCameraRotation;
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest) override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuWidgetClass;
  private:
    bool MovementEnabled = true;
    UPROPERTY()
    UUserWidget *PauseMenuWidget;
    void OnMatchStateChanged(ESTUMatchState State);
    ASTULobbyGameState *STULobbyGameState;
    

};
