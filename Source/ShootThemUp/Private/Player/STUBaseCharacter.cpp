// Shoot THem Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "STUBaseCharacter.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("CameraComponent is %s"), CameraComponent ? TEXT("VALID") : TEXT("NULL"));
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// aCalled to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ASTUBaseCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ASTUBaseCharacter::StopSprint);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    if (Amount > 0 && GetCharacterMovement()->Velocity.Length() > 5)
    {
        isWalking = true;
    }
    else
    {
        isWalking = false;
    }
    AddMovementInput(GetActorForwardVector(), Amount);
}

// Called every frame
void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::StartSprint()
{
    isSprintingPressed = true;
    GetCharacterMovement()->MaxWalkSpeed *= 2.0f; // Double the speed when sprinting
}

void ASTUBaseCharacter::StopSprint()
{
    isSprintingPressed = false;
    GetCharacterMovement()->MaxWalkSpeed /= 2.0f; // Reset the speed when stopping sprint
}

bool ASTUBaseCharacter::IsSprinting()
{
    return isSprintingPressed && isWalking; // Check if the character is both sprinting and walking
}