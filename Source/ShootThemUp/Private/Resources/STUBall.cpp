// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUBall.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/STUBaseCharacter.h"
#include "Components/PlayerUseComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ASTUBall::ASTUBall()
{
	PrimaryActorTick.bCanEverTick = true;
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    PointLight->SetupAttachment(RootComponent);
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->SetRelativeLocation(FVector::ZeroVector);
    InteractionSphere->InitSphereRadius(
        200.f);
    InteractionSphere->SetSphereRadius(200.f);

    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionObjectType(ECC_WorldDynamic);
    InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    InteractionSphere->SetGenerateOverlapEvents(true);
    SetReplicates(true);
    SetReplicateMovement(true);
}



void ASTUBall::BeginPlay()
{
    Super::BeginPlay();
    
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASTUBall::OnInteractionOverlapBegin);
    if (MeshComponent)
    {
        CurrentMaterial = MeshComponent->GetMaterial(0);
        if (!HasAuthority())
        {
            MeshComponent->SetSimulatePhysics(false);
        }
        else
        {
            MeshComponent->SetSimulatePhysics(true);
            MeshComponent->WakeAllRigidBodies();
        }
        
        if (CurrentMaterial)
        {
            UE_LOG(LogTemp, Warning, TEXT("Material found: %s"), *CurrentMaterial->GetName());
        }
    }


}
void ASTUBall::OnInteractionOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                                         UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult &SweepResult)
{
    if (!HasAuthority())
        return;
    if (!OtherActor || OtherActor == this || PlayerController || CooldownRemaining > 0.0f)
        return;

    UE_LOG(LogTemp, Display, TEXT("Ball blocked! Hit actor: %s"), *GetNameSafe(OtherActor));
    ASTUBaseCharacter *Character = Cast<ASTUBaseCharacter>(OtherActor);
    if (Character)
    {
        UPlayerUseComponent *UseComponent = Character->FindComponentByClass<UPlayerUseComponent>();
        if (!UseComponent)
            return;
        UseComponent->Item = this;
        MeshComponent->SetSimulatePhysics(false);
        PlayerController = Cast<APlayerController>(Character->GetController());
        MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void ASTUBall::Use()
{
    Super::Use();

    if (!PlayerController)
        return;
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->WakeAllRigidBodies();
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector SpawnLocation = PlayerController->GetPawn()->GetActorLocation() +
                            PlayerController->GetPawn()->GetActorRotation().RotateVector(RelativeStartImpulseLocation);
    PlayerController = nullptr;
    
   
    FVector LaunchDirection = CameraRotation.Vector();
    
    SetActorLocation(SpawnLocation, false, nullptr, ETeleportType::TeleportPhysics);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    if (MeshComponent && MeshComponent->IsSimulatingPhysics())
    {
        MeshComponent->AddImpulse(LaunchDirection * ForceStrength, NAME_None, true);
    }
    
}

void ASTUBall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (PlayerController && HasAuthority())
    {
        AActor *Pawn = PlayerController->GetPawn();
        if (!Pawn)
            return;
        FVector NewLocation = Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(RelativeStoreLocation);
        SetActorLocation(NewLocation);
    }
}
