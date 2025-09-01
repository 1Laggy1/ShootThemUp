// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;
class USTURespawnComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
    GENERATED_BODY()
  public:
    ASTUAIController();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "EnemyActor";
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName LastEnemyPositionKey = "LastEnemyPosition";
    void OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser);
    virtual void BeginPlay() override;
  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USTUAIPerceptionComponent *STUAIPerceptionComponent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USTURespawnComponent *STURespawnComponent;

    virtual void OnPossess(APawn *InPawn) override;
    virtual void Tick(float DeltaTime) override;

  private:
    AActor *GetFocusOnActor() const;
};
