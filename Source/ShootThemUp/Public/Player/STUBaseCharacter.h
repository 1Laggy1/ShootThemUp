// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/STUPlayerCameraShake.h"
#include "STUCoreTypes.h"
#include "STUBaseCharacter.generated.h"


class USTUHealthActorComponent;
class ASTUBaseWeapon;
class USTUWeaponComponent;
class USoundCue;
class UWidgetComponent;
class USTUPlayerAbilityUseComponent;
class USTUDashAbilityComponent;
UCLASS() class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

  protected:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USTUWeaponComponent* WeaponComponent;
    
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathAnimMontage;
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanTime = 5.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(700.0f, 1400.0f);
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(1.0f, 100.0f);
    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "Paint Color";
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundCue *DeathSound;
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundCue *RespawnSound;
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundCue *DamageSound;
    
    FPlayerInfo* PlayerInfo;

    
    //UPROPERTY(Replicated)
    FRotator AimRotation;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
    virtual void OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser);
    virtual void OnDeath();
    // Called when the game starts or when spawned
    
    virtual void OnHealthChanged(float Health);
    
    
    
  public:

    ASTUBaseCharacter(const FObjectInitializer &ObjInit);
    virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    FPlayerInfo SpawnInfo;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USTUHealthActorComponent *HealthComponent;
    
    void SetPlayerColor(const FLinearColor &Color);
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    void StartSprint();
    void StopSprint();
    UFUNCTION(Server, Reliable)
    void RequestSprintServer(bool Start);
    UFUNCTION(NetMulticast, Reliable)
    void MulticastStartSprint(bool Start);
    AController *GetInstigatorControllerFromDamageCauser(AActor *DamageCauser);
    void InitPlayer();
    void InitAbility();
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.1", ClampMax = "10.0"))
    float SprintSpeedMultiplier = 2.0f;

    bool isWalking;

    bool isSprintingPressed;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    FRotator CalculateAimRotation();
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;
    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsSprinting();
    void UpdateHealthWidgetVisibility(AActor *DamageCauser);

    UPROPERTY(Replicated)
    FLinearColor PlayerColor = FLinearColor::White;
    UPROPERTY(Replicated)
    FString PlayerName = "Player";
    UPROPERTY(Replicated)
    TSubclassOf<USTUPlayerAbilityUseComponent> Ability;
    UPROPERTY(Replicated)
    FString PlayerID = "PlayerID";
    UPROPERTY(Replicated)
    bool MovementEnabled = true;

    //// Ability components. Component Pool for now as I will have easier and faster for me approach then GAS:
    virtual void SetAbilityByClass(TSubclassOf<USTUPlayerAbilityUseComponent> AbilityClass);

    UPROPERTY(Replicated)
    USTUPlayerAbilityUseComponent *ActiveAbilityComponent;

    UPROPERTY()
    TArray<USTUPlayerAbilityUseComponent *> AbilityComponents;
  private:
    
    
    void Landed(const FHitResult &Hit) override;

    
};
