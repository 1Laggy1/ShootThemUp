// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUGoal.generated.h"

class UBoxComponent;
class ASTUGameModeBase;
class ASTUBall;
class UNiagaraSystem;
UCLASS()
class SHOOTTHEMUP_API ASTUGoal : public AActor
{
    GENERATED_BODY()

  public:
    ASTUGoal();
    UBoxComponent *BoxComponent;
    int32 GetTeamID()
    {
        return TeamID;
    }
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem *GoalNiagaraEffect;
  protected:
    virtual void BeginPlay() override;
    UFUNCTION()
    void OnBoxOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    virtual void Goal(ASTUBall *Ball);
    UFUNCTION(NetMulticast, Reliable)
    void GoalFX_Multicast();
  private:
    UPROPERTY(EditAnywhere)
    int32 TeamID;

    ASTUGameModeBase *STUGameModeBase;
};
