// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUExplosiveUse.h"
#include "STUUtils.h"

bool ASTUExplosiveUse::Use(FVector Location, FVector Rotation, AController *InstigatedBy)
{
    bool result = Super::Use(Location, Rotation, InstigatedBy);
    if (result)
    {
        STUUtils::ApplyRadialDamageWithLineOfSight(GetWorld(), GetActorLocation(), ExplosionStats, this, InstigatedBy,
                                                   {});
        
        
    }
    
    return result;
}
