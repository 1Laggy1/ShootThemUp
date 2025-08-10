// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUHealthActorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthActorComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUHealthActorComponent();

    float GetHealth() const
    {
        return Health;
    }

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float MaxHealth = 100.0f;
    virtual void BeginPlay() override;

  private:
    float Health = 0.0f;
};
