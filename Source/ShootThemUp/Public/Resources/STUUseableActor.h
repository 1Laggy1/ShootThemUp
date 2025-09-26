// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUUseableActor.generated.h"

class UWidgetComponent;
class USTUItemUseWidget;
class USoundCue;
UCLASS()
class SHOOTTHEMUP_API ASTUUseableActor : public AActor
{
    GENERATED_BODY()

  public:
    FString GetItemName()
    {
        return ItemName;
    }
    float GetCooldownRemaining()
    {
        return CooldownRemaining;
    }
    float GetCooldown()
    {
        return CooldownTime;
    }
    virtual void ShowItem();
    ASTUUseableActor();
    virtual bool Use(FVector Location, FVector Rotation, AController *InstigatedBy);
    virtual void Tick(float DeltaTime) override;
    void CooldownTick();
  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    FString ItemName;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UWidgetComponent *WidgetComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Use")
    float CooldownTime = 5.0f;
    UPROPERTY(Replicated)
    float CooldownRemaining = 0.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    float TimeAfterHide = 5.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    float HideSpeed = 0.5f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Use")
    USoundCue *UseSound;
    float TimeToHideRemaining = 0.0f;
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
    UFUNCTION(NetMulticast, Reliable)
    virtual void UseFX_Multicast();
    USTUItemUseWidget *ItemWidget;
    
    FTimerHandle CooldownTimerHandle;
  private:
    
    bool IsWidgetShown;
};
