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
class UPlayerUseComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()
  public:
    ASTUPlayerCharacter(const FObjectInitializer &ObjInit); 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USTUPlayerCameraShake *STUPlayerCameraShake;
    UFUNCTION(BlueprintCallable, Category = "Controls")
    void SetBaseSensitivity(float NewSensitivity)
    {
        BaseSensitivity = NewSensitivity;
    }

    UFUNCTION(BlueprintCallable, Category = "Controls")
    void SetAimSensitivityMultiplier(float NewMultiplier)
    {
        AimSensitivityMultiplier = NewMultiplier;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
    float SensitivityMultiplier = 1;
  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent *SpringArmComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent *CameraComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent *CameraCollisionComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UPlayerUseComponent *PlayerUseComponent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float BaseSensitivity = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls", meta = (ClampMin = "0", ClampMax = "3.0"))
    float AimSensitivityMultiplier = 1.0f;
    virtual void OnDeath() override;
    virtual void BeginPlay() override;
    void SnapCamera();
    virtual void OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser) override;
    virtual void Landed(const FHitResult &Hit) override;
    void SprintPressed();
    void SprintUnPressed();
    virtual void StartSprint() override;
    virtual void StopSprint() override;
    virtual void StopFire() override;
    virtual void Zoom(bool Enabled) override;
    void Turn(float Value);
    void LookUp(float Value);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
    TSubclassOf<UCameraShakeBase> LandShake;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
    TSubclassOf<UCameraShakeBase> DamageShake;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
    TSubclassOf<UCameraShakeBase> RunningShake;
    bool bWantToSprint;
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
