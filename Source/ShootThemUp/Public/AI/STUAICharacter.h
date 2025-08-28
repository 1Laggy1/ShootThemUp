// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
  public:
    ASTUAICharacter(const FObjectInitializer &ObjInit);
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree *BehaviorTreeAsset;
    

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent *HealthWidgetComponent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    float HealthVisibilityDistance = 1000.0f;
    virtual void BeginPlay() override;
    virtual void OnDeath() override;
    virtual void OnDamaged(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                           class AController *InstigatedBy, AActor *DamageCauser) override;
    virtual void Tick(float DeltaTime) override;
    void OnHealthChanged(float Health) override;
    void UpdateHealthWidgetVisibility();
};
