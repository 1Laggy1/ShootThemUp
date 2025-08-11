// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthActorComponent;
class UTextRenderComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent *SpringArmComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent *CameraComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USTUHealthActorComponent *HealthComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UTextRenderComponent *HealthTextComponent;
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathAnimMontage;
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanTime = 5.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(700.0f, 1400.0f);
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(1.0f, 100.0f);
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  public:
    ASTUBaseCharacter();
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.1", ClampMax = "10.0"))
    float SprintSpeedMultiplier = 2.0f;

    bool isWalking;
    bool isSprintingPressed;
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

  private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void StartSprint();
    void StopSprint();
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsSprinting();

    void OnDeath();
    void OnHealthChanged(float Health);
    void Landed(const FHitResult &Hit) override;
};
