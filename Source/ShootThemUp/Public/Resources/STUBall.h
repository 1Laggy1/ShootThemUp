// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resources/STUUseableActor.h"
#include "STUBall.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class UMaterialInterface;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBall : public ASTUUseableActor
{
    GENERATED_BODY()

  public:
    ASTUBall();
    APlayerController *PlayerController;
    virtual void Use() override;

  protected:
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    float BallInteractionRadius = 100.0f;
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    float ForceStrength = 1000.0f;
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    FVector RelativeStartImpulseLocation = FVector(0.0f, 0.0f, 50.0f);
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    FVector RelativeStoreLocation = FVector(0.0f, 0.0f, 50.0f);

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent *MeshComponent;

    UPROPERTY(VisibleAnywhere)
    UPointLightComponent *PointLight;

    UMaterialInterface *CurrentMaterial;
    USphereComponent *InteractionSphere;
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    UFUNCTION()
    void OnInteractionOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                                   UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult &SweepResult);
};
