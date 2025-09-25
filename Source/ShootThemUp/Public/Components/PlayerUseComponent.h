// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "PlayerUseComponent.generated.h"

class ASTUUseableActor;
class ASTUBall;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API UPlayerUseComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    UPlayerUseComponent();
    //UPROPERTY(Replicated)
    ASTUUseableActor *Item;
    ASTUUseableActor *HoldItem;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "Stats"))
    float MaxDistance;
    void Use();
    UFUNCTION(Server, Reliable)
    virtual void UseAnItem(FVector Location, FVector Rotation);
    ASTUBall *Ball;
  protected:
    virtual void BeginPlay() override;
    ASTUUseableActor *GetItemInFront();

  private:
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
};
