// Shoot THem Up Game. All Rights Reserved.


#include "Components/PlayerUseComponent.h"
#include "Resources/STUUseableActor.h"

UPlayerUseComponent::UPlayerUseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayerUseComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
ASTUUseableActor *UPlayerUseComponent::GetItemInFront()
{
    AActor *Owner = GetOwner();
    if (!Owner)
        return nullptr;

    APawn *PawnOwner = Cast<APawn>(Owner);
    if (!PawnOwner)
    return nullptr;

    APlayerController *PC = Cast<APlayerController>(PawnOwner->GetController());
    if (!PC)
    return nullptr;

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector Start = CameraLocation;
    FVector End = Start + (CameraRotation.Vector() * MaxDistance);
    
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    if (Hit.bBlockingHit)
    {
        AActor *HitActor = Hit.GetActor();
        if (HitActor)
        {
            ASTUUseableActor *UseableActor = Cast<ASTUUseableActor>(HitActor);
            if (UseableActor)
            {
                APawn *OwnerPawn = Cast<APawn>(GetOwner());
                if (OwnerPawn && OwnerPawn->IsLocallyControlled())
                UseableActor->ShowItem();
                return UseableActor;
            }
        }
    }

    return nullptr;
}
void UPlayerUseComponent::Use()
{
    FVector CameraLocation;
    FRotator CameraRotation;
    Cast<APawn>(GetOwner())->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
    UseAnItem(GetOwner()->GetActorLocation(), CameraRotation.Vector());
}
void UPlayerUseComponent::UseAnItem_Implementation(FVector Location, FVector Rotation)
{
    if (HoldItem)
    {
        HoldItem->Use(Location, Rotation);
        return;
    }
    if (Item)
    {
        Item->Use(Location, Rotation);
    }
}


void UPlayerUseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    Item = GetItemInFront();

    
    
}

