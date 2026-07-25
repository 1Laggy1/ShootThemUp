// Shoot THem Up Game. All Rights Reserved.

#include "Pickups/STUBasePickup.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Pawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All)

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
    
}



// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
    GenerateRotationYaw();
    Super::BeginPlay();
}

void ASTUBasePickup::Interact(AActor *OtherActor)
{
    Super::Interact(OtherActor);
    const auto Pawn = Cast<APawn>(OtherActor);
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
}

void ASTUBasePickup::Cooldown()
{
    Super::Cooldown();
    Respawn();
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(FRotator(0.0f, RotationYaw*DeltaTime, 0.0f));
}

bool ASTUBasePickup::GivePickupTo(APawn *PlayerPawn)
{
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    UE_LOG(LogBasePickup, Display, TEXT("Pickuptaken:"));


    GetRootComponent()->SetVisibility(false, true);

    if (GetWorld() && PickupSound)
    {
        FVector SoundLocation = GetActorLocation();
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }
}

void ASTUBasePickup::Respawn()
{
    GenerateRotationYaw();

    if (!GetRootComponent())
        return;

    GetRootComponent()->SetVisibility(true, true);
}

void ASTUBasePickup::GenerateRotationYaw()
{
    RotationYaw = FMath::RandRange(30.0f, 90.0f);
}
