// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUExplosiveUse.h"
#include "STUUtils.h"
#include "NiagaraFunctionLibrary.h" 
bool ASTUExplosiveUse::Use(FVector Location, FVector Rotation, AController *InstigatedBy)
{
    bool result = Super::Use(Location, Rotation, InstigatedBy);
    if (result)
    {
        STUUtils::ApplyRadialDamageWithLineOfSight(GetWorld(), GetActorLocation(), ExplosionStats, this, InstigatedBy,
                                                   {});
        FVector WorldOffset = GetActorRotation().RotateVector(RelativeExplostionPos);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, GetActorLocation() + WorldOffset,
                                                       GetActorRotation() + FRotator(90.f, 0.f, 0.f));
    }
    
    return result;
}
