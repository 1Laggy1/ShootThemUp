// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resources/STUUseableActor.h"
#include "Components/WidgetComponent.h"
#include "STUBall.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class UMaterialInterface;
class USphereComponent;
class ASTUBaseCharacter;
class UMaterialInstanceDynamic;
class UWidgetComponent;
class USTUBallWidget;

UCLASS()
class SHOOTTHEMUP_API ASTUBall : public ASTUUseableActor
{
    GENERATED_BODY()

  public:
    ASTUBall();
    UPROPERTY(Replicated)
    ASTUBaseCharacter *PlayerCharacter;
    ASTUBaseCharacter *PreviousPlayerCharacter;
    virtual bool Use(FVector Location, FVector Rotation) override;
    UFUNCTION()
    void CharacterDied();
    void PullBall(FVector Position);
  protected:
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    float MaxDistance = 3000.0f;
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    float MaxPullStrength = 2000.0f;
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    float MinPullStrength = 500.0f;
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    float BallInteractionRadius = 100.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "STU")
    float BallInteractionCooldown = 0.3f;
    float BallInteractionCooldownRemaining = 0.3f;
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UWidgetComponent *BallWidgetComponent;

    UMaterialInterface *CurrentMaterial;
    USphereComponent *InteractionSphere;

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const;
    UFUNCTION()
    void OnInteractionOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                                   UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult &SweepResult);
    UFUNCTION(NetMulticast, Reliable)
    void SetReplicated(bool set);

  private:
    UFUNCTION(NetMulticast, Reliable)
    void ChangeBallColor(FLinearColor NewColor);
    void PickUpBall(ASTUBaseCharacter *Character);
    void UnAttach();
    
    UPROPERTY()
    UMaterialInstanceDynamic *DynamicMaterial;
    USTUBallWidget *BallWidget;
};
