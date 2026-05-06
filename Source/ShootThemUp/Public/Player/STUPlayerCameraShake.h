// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUPlayerCameraShake.generated.h"


class UCameraComponent;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUPlayerCameraShake : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUPlayerCameraShake();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent *CameraComponent;
  protected:
    virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shake")
    TSubclassOf<UCameraShakeBase> DefaultCameraShake;

  public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;
    void PlayCameraShake(APlayerController *pc, TSubclassOf<UCameraShakeBase> CameraShake, float Scale = 1.f);
};
