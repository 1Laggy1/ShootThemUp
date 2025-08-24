// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
  public:
    ASTUAICharacter(const FObjectInitializer &ObjInit);
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree *BehaviorTreeAsset;
    

  protected:
    virtual void OnDeath() override;
    virtual void OnDamaged(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                           class AController *InstigatedBy, AActor *DamageCauser) override;
    virtual void Tick(float DeltaTime) override;
};
