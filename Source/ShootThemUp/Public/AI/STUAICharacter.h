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
    
    
    virtual void BeginPlay() override;
    virtual void OnDeath() override;
    virtual void OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser) override;
    virtual void Tick(float DeltaTime) override;
    void OnHealthChanged(float Health) override;
};
