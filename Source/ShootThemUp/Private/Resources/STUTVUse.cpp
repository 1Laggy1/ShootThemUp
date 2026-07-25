// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUTVUse.h"
#include "STUUtils.h"

bool ASTUTVUse::Use(FVector Location, FVector Rotation, AController *InstigatedBy)
{
    bool result = Super::Use(Location, Rotation, InstigatedBy);
    if (result)
    {
        InstigatorController = InstigatedBy;
        TimeToDamageRemaining = 0.f;
        GetWorld()->GetTimerManager().SetTimer(DamageAgainTimerHandle, 
                                               this,
                                               &ASTUTVUse::DamageAgain,
                                               TimeBetweenDamage,
                                               true
        );
    }
    return result;
}

void ASTUTVUse::DamageAgain()
{
    if (InstigatorController && TimeToDamageRemaining < TimeToDamage)
    {
        TimeToDamageRemaining += TimeBetweenDamage;
        STUUtils::ApplyRadialDamageWithLineOfSight(GetWorld(), GetActorLocation(), ExplosionStats, this,
                                                   InstigatorController, {});
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(DamageAgainTimerHandle);
    }
    
}
