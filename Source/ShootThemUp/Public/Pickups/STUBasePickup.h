// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
    GENERATED_BODY()

  public:
    ASTUBasePickup();

  protected:
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent *CollisionComponent;
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    float RespawnTime = 5.0f;
    virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

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
