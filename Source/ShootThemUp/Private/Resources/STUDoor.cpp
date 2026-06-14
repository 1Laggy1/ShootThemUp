// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUDoor.h"

ASTUDoor::ASTUDoor()
{
    	// Could be too high or too low. Needs testing
    	SetNetUpdateFrequency(100.f);

	SetMinNetUpdateFrequency(30.f);
}

void ASTUDoor::BeginPlay()
{
    Super::BeginPlay();

    ClosedLocation = GetActorLocation();
    OpenedLocation = ClosedLocation + OpenDifference;
}

void ASTUDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (AnimDone)
        return;

    FVector Target = IsClosed ? ClosedLocation : OpenedLocation;
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


bool ASTUDoor::Use(FVector Location, FVector Rotation, AController *InstigatedBy)
{

    bool IsUsed = Super::Use(Location, Rotation, InstigatedBy);
    if (!IsUsed)
        return false;
    IsClosed = !IsClosed;
    AnimDone = false;
    return true;
}
