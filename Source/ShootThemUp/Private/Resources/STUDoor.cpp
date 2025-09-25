// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUDoor.h"

void ASTUDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (AnimDone)
        return;

    FVector Target = IsClosed ? ClosedLocation : OpenLocation;
    FVector Current = GetActorLocation();

    FVector Direction = (Target - Current).GetSafeNormal();
    float Distance = FVector::Dist(Target, Current);

    float MoveStep = Speed * DeltaTime;

    if (MoveStep >= Distance)
    {
        SetActorLocation(Target);
        AnimDone = true;
    }
    else
    {
        SetActorLocation(Current + Direction * MoveStep);
    }
}


bool ASTUDoor::Use(FVector Location, FVector Rotation)
{

    bool IsUsed = Super::Use(Location, Rotation);
    if (!IsUsed)
        return false;
    AnimDone = false;
    IsClosed = !IsClosed;
    return true;
}
