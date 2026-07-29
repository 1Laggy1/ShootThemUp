#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUPlayerUIComponent.generated.h"

class UWidgetComponent;
class USTUPlayerUIWidget;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUPlayerUIComponent : public UActorComponent
{
    GENERATED_BODY()

    public:
        USTUPlayerUIComponent();
        UWidgetComponent* GetPlayerUIWidgetComponent()//
        {
            return PlayerUIWidgetComponent;
        }

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        float UiVisibilityDistance = 10000.0f;

        void SetPlayerName(FString PlayerName);
        void SetPlayerColor(const FLinearColor& Color);

        void OnDamaged(AActor *DamagedActor, float HealthPercent, AActor *DamageCauser);

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UWidgetComponent *PlayerUIWidgetComponent;
        USTUPlayerUIWidget *PlayerUIWidget;
        FTimerHandle HideUITimerHandle;
        void HideUI();
        void ShowUI();
        void ToShowOrNotToShow(AActor *DamageCauser);

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        float PercentVisibilityThreshold = 0.8;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        float PercentColorThreshold = 0.3f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        float TimeToShow = 5;

        void UpdateHealthWidget(AActor* DamageCauser, float NewHealth);

    private:
        virtual void BeginPlay() override;
        FTimerHandle OcclusionTimerHandle;
        void CheckOcclusion();


};