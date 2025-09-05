// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "Components/ProgressBar.h"
#include "STUGameStateBase.h"
#include "STUCoreTypes.h"

#include "Player/STUPlayerController.h"
DEFINE_LOG_CATEGORY_STATIC(LogHudWidget, All, All)

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (GetOwningPlayer() && Cast<ASTUPlayerController>(GetOwningPlayer()))
    {
        Cast<ASTUPlayerController>(GetOwningPlayer())->OnNewPawnEvent.AddUObject(
            this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
    
    CurrentGamemodeState = Cast<ASTUGameStateBase>(GetWorld()->GetGameState());
    return;
}
void USTUPlayerHUDWidget::OnNewPawn(APawn *NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(NewPawn);
    FString yes = HealthComponent ? FString("Yes") : FString("No");
    UE_LOG(LogHudWidget, Warning, TEXT("NewPawn= %s, HealthComponent = %s"), *NewPawn->GetFullName(), *yes);
    if (HealthComponent)
    {
        HealthComponent->OnDamaged.AddUObject(this, &USTUPlayerHUDWidget::OnDamaged);
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }
    UpdateHealthBar();
}
void USTUPlayerHUDWidget::OnHealthChanged(float Health)
{
    UpdateHealthBar();
}
void USTUPlayerHUDWidget::UpdateHealthBar()
{
    if (!HealthProgressBar)
        return;
    HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
}
void USTUPlayerHUDWidget::OnDamaged(AActor * DamagedActor, float Damage, AActor *DamageCauser)
{
    if (!IsAnimationPlaying(DamageAnimation))
    {
        PlayAnimation(DamageAnimation);
    }
}

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(GetOwningPlayerPawn());
    if (!HealthComponent)
        return 0.0f;
    return HealthComponent->GetHealthPercent();
}

FString USTUPlayerHUDWidget::GetCurrentAmmo() const
{
    FAmmoData AmmoData;
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent || !WeaponComponent->GetCurrentAmmoData(AmmoData))
    {
        return "0/0";
    }
    FString AmmoInfo = FString::FromInt(AmmoData.Bullets) + " / ";
    AmmoInfo += AmmoData.Infinite ? FString::Chr(0x221E) : FString::FromInt(AmmoData.Clips);
    //UE_LOG(LogHudWidget, Warning, *Ammo);
    return AmmoInfo;
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent)
        return false;
    WeaponComponent->GetWeaponUIData(UIData);
    if (UIData.MainIcon && UIData.CrosshairIcon)
        return true;
    else
        return false;

}

bool USTUPlayerHUDWidget::isPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->isDead();
}

bool USTUPlayerHUDWidget::isPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    const auto PlayerController = Cast<ASTUPlayerController>(Controller);
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    return PlayerState && PlayerState->GetSTUPlayerState() == STUPlayerStateEnum::Spectating;
}

FString USTUPlayerHUDWidget::GetRoundsInfo()
{
    if (!GetWorld() || !CurrentGamemodeState)
        return "Rounds: 0/0";
    FString RoundsInfo = "Round: ";
    FGameData GameData = CurrentGamemodeState->GetGameData();
    RoundsInfo = RoundsInfo + FString::FromInt(CurrentGamemodeState->GetCurrentRound()) + "/" +
                 FString::FromInt(GameData.RoundsNum);
    return RoundsInfo;
}

FString USTUPlayerHUDWidget::GetKills()
{
    if (!GetWorld() || !CurrentGamemodeState || !GetOwningPlayerPawn() || !GetOwningPlayerPawn()->Controller)
        return "Kills: 0";
    const auto PlayerState = Cast<ASTUPlayerState>(GetOwningPlayerPawn()->Controller->PlayerState);
    if (!PlayerState)
        return "Kills: 0";
    FString Kills = "Kills: ";
    Kills += FString::FromInt(PlayerState->GetKillsNum());
    return Kills;
}

FString USTUPlayerHUDWidget::GetCurrentTime()
{
    if (!GetWorld() || !CurrentGamemodeState)
        return "00:00";
    FString Time;
    int32 Minutes = CurrentGamemodeState->GetRoundCountDown() / 60;
    int32 Seconds = CurrentGamemodeState->GetRoundCountDown() % 60;  
    Time = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
    return Time;
}



