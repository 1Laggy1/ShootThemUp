// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resources/STUInteractable.h"
#include "STUBasePickup.generated.h"


class USoundBase;
UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public ASTUInteractable
{
    GENERATED_BODY()

  public:
    ASTUBasePickup();
    

  protected:
    
    
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase *PickupSound;
    virtual void BeginPlay() override;
    virtual void Interact(AActor *OtherActor) override;
    virtual void Cooldown() override;

  public:
    virtual void Tick(float DeltaTime) override;
    virtual bool GivePickupTo(APawn *PlayerPawn);

  private:
    float RotationYaw = 0.0f;
    float RotationSpeed = 40.0f;


    void PickupWasTaken();
    void Respawn();
    void GenerateRotationYaw();
    
};
