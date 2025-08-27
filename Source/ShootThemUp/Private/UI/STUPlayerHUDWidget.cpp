// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
DEFINE_LOG_CATEGORY_STATIC(LogHudWidget, All, All)

bool USTUPlayerHUDWidget::Initialize()
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(GetOwningPlayerPawn());
    if (HealthComponent)
    {
        HealthComponent->OnDamaged.AddUObject(this, &USTUPlayerHUDWidget::OnDamaged);
    }
    CurrentGamemode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    return Super::Initialize();
}

void USTUPlayerHUDWidget::OnDamaged(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                                    class AController *InstigatedBy, AActor *DamageCauser)
{
    OnTakeDamage();
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
    return true;

}

bool USTUPlayerHUDWidget::isPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->isDead();
}

bool USTUPlayerHUDWidget::isPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

FString USTUPlayerHUDWidget::GetRoundsInfo()
{
    if (!GetWorld() || !CurrentGamemode)
        return "Rounds: 0/0";
    FString RoundsInfo = "Rounds: ";
    FGameData GameData = CurrentGamemode->GetGameData();
    RoundsInfo = RoundsInfo + FString::FromInt(CurrentGamemode->GetCurrentRound()) + "/" + FString::FromInt(GameData.RoundsNum);
    return RoundsInfo;
}

FString USTUPlayerHUDWidget::GetKills()
{
    if (!GetWorld() || !CurrentGamemode || !GetOwningPlayerPawn() || !GetOwningPlayerPawn()->Controller)
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
    if (!GetWorld() || !CurrentGamemode)
        return "00:00";
    FString Time;
    int32 Minutes = CurrentGamemode->GetRoundCountDown() / 60;
    int32 Seconds = CurrentGamemode->GetRoundCountDown() % 60;  
    Time = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
    return Time;
}



