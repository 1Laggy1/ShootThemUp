// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "PlayerUseComponent.generated.h"

class ASTUUseableActor;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API UPlayerUseComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    UPlayerUseComponent();
    //UPROPERTY(Replicated)
    ASTUUseableActor *Item;
    void Use();
    UFUNCTION(Server, Reliable)
    virtual void UseAnItem(FVector Location, FVector Rotation);
  protected:
    virtual void BeginPlay() override;
    

  private:
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
};
