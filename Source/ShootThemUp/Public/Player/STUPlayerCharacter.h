// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

/**
 *
 */

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()
  public:
    ASTUPlayerCharacter(const FObjectInitializer &ObjInit);

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent *SpringArmComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent *CameraComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent *CameraCollisionComponent;
    virtual void OnDeath() override;
    virtual void BeginPlay() override;
    void SnapCamera();
    virtual void OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser) override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USTUPlayerCameraShake *STUPlayerCameraShake;

  public:
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

  private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent*
                                       OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnCameraCollisionEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
                                     UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);
    void CheckCameraOverlap();
};
