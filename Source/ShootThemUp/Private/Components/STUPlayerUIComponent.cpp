#include "Components/STUPlayerUIComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUPlayerUIWidget.h"
#include "STUUtils.h"

USTUPlayerUIComponent::USTUPlayerUIComponent()
{
    PlayerUIWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("PlayerUIWidgetComponent");
    PlayerUIWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

void USTUPlayerUIComponent::BeginPlay()
{
    Super::BeginPlay();
    PlayerUIWidgetComponent->InitWidget();
    PlayerUIWidget = Cast<USTUPlayerUIWidget>(PlayerUIWidgetComponent->GetUserWidgetObject());
    HideUI();
}

void USTUPlayerUIComponent::ShowUI()
{
    if (PlayerUIWidgetComponent)
    {
        PlayerUIWidgetComponent->SetVisibility(true, true);
    }
   
    GetWorld()->GetTimerManager().SetTimer(HideUITimerHandle, this, &USTUPlayerUIComponent::HideUI, 
        TimeToShow,
        false
    );
}

void USTUPlayerUIComponent::HideUI()
{
    if (PlayerUIWidgetComponent)
    {
        PlayerUIWidgetComponent->SetVisibility(false, true);
    }
    
}
void USTUPlayerUIComponent::ToShowOrNotToShow(AActor *DamageCauser)
{
    if (DamageCauser)
    {
        const auto ControllerCauser = STUUtils::GetInstigatorControllerFromDamageCauser(DamageCauser);
        if (ControllerCauser)
        {
            ShowUI();
        }
    }
}
void USTUPlayerUIComponent::UpdateHealthWidget(AActor *DamageCauser, float HealthPercent)
{
    if (!GetOwner() || !PlayerUIWidgetComponent)
        return;
    if (PlayerUIWidget)
    {
        PlayerUIWidget->SetHealthPercent(HealthPercent);
    }
    ToShowOrNotToShow(DamageCauser);
    
}

void USTUPlayerUIComponent::OnDamaged(AActor *DamagedActor, float HealthPercent, AActor *DamageCauser)
{
    UpdateHealthWidget(DamageCauser, HealthPercent);
}