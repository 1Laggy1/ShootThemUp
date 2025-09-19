// Shoot THem Up Game. All Rights Reserved.


#include "Player/STUPlayerCharacter.h"
#include "Player/STUPlayerCameraShake.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/PlayerUseComponent.h"

#include "Player/STUPlayerController.h"
DEFINE_LOG_CATEGORY_STATIC(STUPlayerCharacter, All, All);
ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    STUPlayerCameraShake = CreateDefaultSubobject<USTUPlayerCameraShake>("Player Camera Shake");
    PlayerUseComponent = CreateDefaultSubobject<UPlayerUseComponent>("PlayerUseComponent");
    
}
void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(CameraCollisionComponent);
    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(
        this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(
        this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}
void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (!IsLocallyControlled())
        return;
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ASTUPlayerCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ASTUPlayerCharacter::StopSprint);
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", EInputEvent ::IE_Released, WeaponComponent,
                                     &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent,
                                     &USTUWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
    PlayerInputComponent->BindAction("SnapCamera", EInputEvent::IE_Pressed, this, &ASTUPlayerCharacter::SnapCamera);

    DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", EInputEvent::IE_Pressed, WeaponComponent,
                                                          &USTUWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", EInputEvent::IE_Released, WeaponComponent,
                                                          &USTUWeaponComponent::Zoom, false);
    PlayerInputComponent->BindAction("UseItem", EInputEvent::IE_Pressed, PlayerUseComponent,
                                     &UPlayerUseComponent::Use);
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{

     /*if (Amount > 0 && GetCharacterMovement()->Velocity.Length() > 5)
    {
         isWalking = true;
     }
     else
    {
         isWalking = false;
     }*/
    AddMovementInput(GetActorForwardVector(), Amount);
}

// Called every frame
void ASTUPlayerCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                                        UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
                                                        bool bFromSweep, const FHitResult &SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                                      UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);
    TArray<USceneComponent *> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);
    for (auto MeshChild : MeshChildren)
    {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if (MeshChildGeometry)
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}

void ASTUPlayerCharacter::OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser)
{
    Super::OnDamaged(DamagedActor, Damage, DamageCauser);
    STUPlayerCameraShake->PlayCameraShake(Cast<APlayerController>(GetController()));
}

void ASTUPlayerCharacter::OnDeath()
{
    
    WeaponComponent->Zoom(false);
    if (Controller)
    {
    //    Cast<ASTUPlayerController>(Controller)->SetPreviousPosition(GetActorLocation());

    
    if (Cast<ASTUPlayerController>(Controller))
    {
        Cast<ASTUPlayerController>(Controller)
            ->SetPreviousCameraPosition(CameraComponent->GetComponentLocation(),
                                        CameraComponent->GetComponentRotation());
    }
    //    
    }
    Super::OnDeath();
    //UE_LOG(STUPlayerCharacter, Display, TEXT("Player %s is dead"), *GetName());
    
}



void ASTUPlayerCharacter::SnapCamera()
{
    SpringArmComponent->SocketOffset = FVector(SpringArmComponent->SocketOffset.X, -SpringArmComponent->SocketOffset.Y,
                                               SpringArmComponent->SocketOffset.Z);
}
