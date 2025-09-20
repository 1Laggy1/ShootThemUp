// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUBallSpawn.h"


ASTUBallSpawn::ASTUBallSpawn()
{
    USceneComponent *SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneComp;
}

