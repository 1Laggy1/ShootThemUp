// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUBall.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/STUBaseCharacter.h"
#include "Components/PlayerUseComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "STUUtils.h"

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
        MeshComponent->SetSimulatePhysics(true);
        MeshComponent->WakeAllRigidBodies();
        
        
        if (CurrentMaterial)
        {
            UE_LOG(LogTemp, Warning, TEXT("Material found: %s"), *CurrentMaterial->GetName());
            DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
            MeshComponent->SetMaterial(0, DynamicMaterial);
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
    if (!OtherActor || OtherActor == this || PlayerCharacter || CooldownRemaining > 0.0f)
        return;

    UE_LOG(LogTemp, Display, TEXT("Ball blocked! Hit actor: %s"), *GetNameSafe(OtherActor));
    ASTUBaseCharacter *Character = Cast<ASTUBaseCharacter>(OtherActor);
    if (Character)
    {
        PickUpBall(Character);
    }
}

void ASTUBall::ChangeBallColor_Implementation(FLinearColor NewColor)
{
    if (DynamicMaterial && PointLight)
    {
        static float OriginalIntensity = PointLight->Intensity;
        DynamicMaterial->SetScalarParameterValue(FName("Glow"), 30.0f);
        float Luminance = 0.299f * NewColor.R + 0.587f * NewColor.G + 0.114f * NewColor.B;

        float TargetLuminance = 0.5f;
        if (Luminance > 0.0f)
        {
            float Scale = TargetLuminance / Luminance;
            NewColor.R *= Scale;
            NewColor.G *= Scale;
            NewColor.B *= Scale;
        }
        DynamicMaterial->SetVectorParameterValue(FName("Color"), NewColor);
        PointLight->SetLightColor(NewColor.ToFColor(true));
        PointLight->SetIntensity(OriginalIntensity);
        
    }

    
}

void ASTUBall::PickUpBall(ASTUBaseCharacter *Character)
{
    UPlayerUseComponent *UseComponent = Character->FindComponentByClass<UPlayerUseComponent>();
    if (!UseComponent)
        return;
    UseComponent->Item = this;
    MeshComponent->SetSimulatePhysics(false);
    PlayerCharacter = Character;
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    APlayerController *PlayerController = Cast<APlayerController>(Character->GetController());
    USTUGameInstance *STUGameInstance = Cast<USTUGameInstance>(GetWorld()->GetGameInstance());
    if (PlayerController && STUGameInstance)
    {

        FPlayerInfo *PlayerInfo = STUUtils::FindPlayerByPlayerID(
            PlayerController->PlayerState->GetUniqueId()->ToString(), STUGameInstance->Teams);
        FLinearColor PlayerColor = PlayerInfo->Color;
        ChangeBallColor(PlayerColor);
    }
    SetReplicated(false);
}

void ASTUBall::SetReplicated_Implementation(bool set)
{
    SetReplicateMovement(set);
    MeshComponent->SetCollisionEnabled(set ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);

}

void ASTUBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTUBall, PlayerCharacter);
}

void ASTUBall::Use(FVector Location, FVector Rotation)
{
    Super::Use(Location, Rotation);

    if (!PlayerCharacter)
        return;
    SetReplicated(true);
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->WakeAllRigidBodies();
    
    FVector SpawnLocation = Location +
                            PlayerCharacter->GetActorRotation().RotateVector(RelativeStartImpulseLocation);
    PlayerCharacter = nullptr;

    FRotator Rotator = Rotation.Rotation();
    FVector LaunchDirection = Rotator.Vector();

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
    if (PlayerCharacter)
    {
        AActor *Pawn = PlayerCharacter;
        if (!Pawn)
            return;
        FVector NewLocation = Pawn->GetActorLocation() + Pawn->GetActorRotation().RotateVector(RelativeStoreLocation);
        SetActorLocation(NewLocation);
    }
}
