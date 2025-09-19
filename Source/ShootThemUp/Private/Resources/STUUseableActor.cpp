// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUUseableActor.h"

ASTUUseableActor::ASTUUseableActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASTUUseableActor::Use(FVector Location, FVector Rotation)
{
    if (CooldownRemaining > 0.0f)
        return;
	CooldownRemaining = CooldownTime;
    GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ASTUUseableActor::CooldownTick, 0.1f, true);
	
}


void ASTUUseableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTUUseableActor::CooldownTick()
{
	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining -= 0.1f;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
    }

}

