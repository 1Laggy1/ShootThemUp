// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUInteractable.generated.h"

class UBoxComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUInteractable();
    bool CouldBeTaken();

protected:
    UPROPERTY(EditAnywhere, Category = "Pickup")
    float CooldownTime = 5.0f;
    
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    UBoxComponent *CollisionComponent;
    FTimerHandle CooldownTimerHandle;
    void NotifyActorBeginOverlap(AActor *OtherActor) override;
    virtual void Interact(AActor *OtherActor);
    virtual void Cooldown();
 

public:	

};
