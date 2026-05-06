// Shoot THem Up Game. All Rights Reserved.

#include "UI/STUPlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/STUHealthActorComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Player/STUPlayerState.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.h"
#include "STUGameStateBase.h"
#include "STUUtils.h"
#include "Player/STUBaseCharacter.h"
#include "Components/TextBlock.h"
#include "Player/STUPlayerController.h"
#include "Components/Abilities/STUPlayerAbilityUseComponent.h"
#include "Components/Image.h"
DEFINE_LOG_CATEGORY_STATIC(LogHudWidget, All, All)

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (GetOwningPlayer() && Cast<ASTUPlayerController>(GetOwningPlayer()))
    {
        Cast<ASTUPlayerController>(GetOwningPlayer())->OnNewPawnEvent.AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }

    CurrentGamemodeState = Cast<ASTUGameStateBase>(GetWorld()->GetGameState());
    CurrentGamemodeState->OnTimerChanged.AddUObject(this, &USTUPlayerHUDWidget::OnTimerChanged);
    return;
}
void USTUPlayerHUDWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (MyAbility && GetWorld() && AbilityCountText)
    {
        AbilityProgressBar->SetPercent(MyAbility->GetAbilityCooldownRemainingPrecents());
        AbilityCountText->SetText(FText::FromString(FString::FromInt(MyAbility->GetUseCount())));
    }
}
void USTUPlayerHUDWidget::OnNewPawn(APawn *NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthActorComponent>(NewPawn);
    FString yes = HealthComponent ? FString("Yes") : FString("No");
    UE_LOG(LogHudWidget, Warning, TEXT("NewPawn= %s, HealthComponent = %s"), *NewPawn->GetFullName(), *yes);
    isPlayerDead = false;
    if (HealthComponent)
    {
        HealthComponent->OnDamaged.AddUObject(this, &USTUPlayerHUDWidget::OnDamaged);
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
        HealthComponent->OnDeath.AddUObject(this, &USTUPlayerHUDWidget::OnPlayerDeath);
    }
    UpdateHealthBar();
    const auto CharacterPawn = Cast<ASTUBaseCharacter>(NewPawn);
    if (CharacterPawn)
    {
        if (CharacterPawn->AbilityClass)
        {
            const auto Compon = CharacterPawn->GetComponentByClass(CharacterPawn->AbilityClass);
            if (Compon)
            {
                MyAbility = Cast<USTUPlayerAbilityUseComponent>(Compon);
                if (AbilityImage && MyAbility->GetAbilityIcon())
                {
                    FSlateBrush Brush;
                    Brush.SetResourceObject(MyAbility->GetAbilityIcon());
                    AbilityImage->SetBrush(Brush);
                }
            }
        }
    }
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
void USTUPlayerHUDWidget::OnDamaged(AActor *DamagedActor, float Damage, AActor *DamageCauser)
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
    // UE_LOG(LogHudWidget, Warning, *Ammo);
    return AmmoInfo;
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData &UIData) const
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
    FString RoundsInfo = "";
    RoundsInfo += FString::FromInt(CurrentGamemodeState->TeamsStats[0].Score);
    RoundsInfo += "/";
    RoundsInfo += FString::FromInt(CurrentGamemodeState->TeamsStats[1].Score);
    return RoundsInfo;
}

FString USTUPlayerHUDWidget::GetKills()
{
    if (!GetWorld() || !CurrentGamemodeState ||
        !GetWorld()->GetFirstPlayerController()) //! GetOwningPlayerPawn() || !GetOwningPlayerPawn()->Controller)
        return "Kills: 0";
    const auto PlayerState = Cast<ASTUPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
    if (!PlayerState)
        return "Kills: 0";
    FString Kills = "Kills: ";
    Kills += FString::FromInt(PlayerState->GetKillsNum());
    return Kills;
}

void USTUPlayerHUDWidget::OnTimerChanged(int32 CurrentTime)
{
    if (CurrentTime > 0)
    {
        TimerText->SetVisibility(ESlateVisibility::Visible);
        PlayAnimation(TimerAnimation);
        TimerText->SetText(FText::FromString(FString::FromInt(CurrentTime)));
    }
    else
    {
        TimerText->SetVisibility(ESlateVisibility::Visible);
    }
}

FString USTUPlayerHUDWidget::GetCurrentTime()
{
    if (!GetWorld() || !CurrentGamemodeState)
        return "00:00";
    FString Time;
    int32 Minutes = CurrentGamemodeState->GetGameCountDown() / 60;
    int32 Seconds = CurrentGamemodeState->GetGameCountDown() % 60;
    Time = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
    return Time;
}
