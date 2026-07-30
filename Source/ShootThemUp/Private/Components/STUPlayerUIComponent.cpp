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

    GetWorld()->GetTimerManager().SetTimer(
        OcclusionTimerHandle, 
        this, 
        &USTUPlayerUIComponent::CheckOcclusion, 
        0.1f, 
        true
    );
}

void USTUPlayerUIComponent::HideUI()
{
    if (PlayerUIWidgetComponent)
    {
        PlayerUIWidgetComponent->SetVisibility(false, true);
        GetWorld()->GetTimerManager().ClearTimer(OcclusionTimerHandle);
    }
    GetWorld()->GetTimerManager().ClearTimer(OcclusionTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(HideUITimerHandle);
    
}
void USTUPlayerUIComponent::ToShowOrNotToShow(AActor *DamageCauser)
{
    if (DamageCauser)
    {
        const auto ControllerCauser = STUUtils::GetInstigatorControllerFromDamageCauser(DamageCauser);
        
        if (ControllerCauser && ControllerCauser->IsLocalController())
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

void USTUPlayerUIComponent::SetPlayerName(FString PlayerName)
{
    if (!PlayerUIWidget)
    {
        if (PlayerUIWidgetComponent)
        {
            PlayerUIWidgetComponent->InitWidget();
            PlayerUIWidget = Cast<USTUPlayerUIWidget>(PlayerUIWidgetComponent->GetUserWidgetObject());
        }
    }

    if (PlayerUIWidget)
    {
        PlayerUIWidget->SetPlayerName(PlayerName);
    }
}
void USTUPlayerUIComponent::SetPlayerColor(const FLinearColor& Color)
{
    if (!PlayerUIWidget)
    {
        if (PlayerUIWidgetComponent)
        {
            PlayerUIWidgetComponent->InitWidget();
            PlayerUIWidget = Cast<USTUPlayerUIWidget>(PlayerUIWidgetComponent->GetUserWidgetObject());
        }
    }

    if (PlayerUIWidget)
    {
        PlayerUIWidget->SetPlayerColor(Color);
    }
}

void USTUPlayerUIComponent::OnDamaged(AActor *DamagedActor, float HealthPercent, AActor *DamageCauser)
{
    UpdateHealthWidget(DamageCauser, HealthPercent);
}
void USTUPlayerUIComponent::CheckOcclusion()
{
    if (!GetOwner() || !GetWorld() || !PlayerUIWidgetComponent) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC || !PC->PlayerCameraManager) return;

    FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
    FVector TargetLocation = GetOwner()->GetActorLocation()+ FVector(0,0,50);

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    
    if (APawn* LocalPawn = PC->GetPawn())
    {
        CollisionParams.AddIgnoredActor(LocalPawn);
    }

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        CameraLocation,
        TargetLocation,
        ECC_Visibility,
        CollisionParams
    );

    if (bHit)
    {
        PlayerUIWidgetComponent->SetVisibility(false, true);
    }
    else
    {
        PlayerUIWidgetComponent->SetVisibility(true, true);
    }
}