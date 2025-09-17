// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBall.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class UMaterialInterface;

UCLASS()
class SHOOTTHEMUP_API ASTUBall : public AActor
{
	GENERATED_BODY()

  public:
    ASTUBall();

  protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent *MeshComponent;

    UPROPERTY(VisibleAnywhere)
    UPointLightComponent *PointLight;

    UMaterialInterface *CurrentMaterial;
    virtual void BeginPlay() override;

};
