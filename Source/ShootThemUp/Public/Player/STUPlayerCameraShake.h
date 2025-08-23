// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUPlayerCameraShake.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUPlayerCameraShake : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUPlayerCameraShake();

  protected:
    virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shake")
    TSubclassOf<UCameraShakeBase> DefaultCameraShake;

  public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;
    void PlayCameraShake(APlayerController* pc);
};
