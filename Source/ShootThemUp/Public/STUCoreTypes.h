#pragma once
#include "STUCoreTypes.generated.h"
// weapon
class ASTUBaseWeapon;
class USoundCue;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipSignature, ASTUBaseWeapon *);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "0"))
    int32 Bullets;
    UPROPERTY(EditAnywhere, Category = "Stats", meta = (EditCondition = "!Infinite"))
    int32 Clips;
    UPROPERTY(EditAnywhere, Category = "Stats")
    bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage *ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D *MainIcon;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D *CrosshairIcon;
};

// health
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)
    DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDamaged, AActor *, float, AActor *)

    /// VFX
    class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UMaterialInterface *Material;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(10.0f);
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 5.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 0.7f;
};
USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem *NiagaraEffect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FDecalData DecalData;
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundCue *ImpactSound;
};
USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1"))
    int32 PlayersNum = 2;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1"))
    int32 RoundsNum = 4;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3"))
    /// <summary>
    /// in seconds
    /// </summary>
    int32 RoundTime = 10;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor DefaultTeamColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<FLinearColor> TeamColors;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "0"))
    int32 RespawnTime = 5; // in seconds
};

UENUM(BlueprintType)
enum class ESTUMatchState : uint8
{
    WaitingToStart = 0,
    InProgress,
    Pause,
    GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangeSignature, ESTUMatchState)

    // ui
USTRUCT(BlueprintType) 
struct FStatRowInfo
{
    GENERATED_USTRUCT_BODY()
    /*FStatRowInfo()
    {
    }
    FStatRowInfo(FPlayerStats &PlayerStats)
    {
        PlayerNameText = PlayerStats.PlayerName;
        KillsText = FString::FromInt(PlayerStats.Kills);
        DeathsText = FString::FromInt(PlayerStats.Deaths);
        TeamText = FString::FromInt(PlayerStats.TeamID);
        TeamColor = PlayerStats.TeamColor;
    }*/
    UPROPERTY()
    FString PlayerNameText;
    UPROPERTY()
    FString KillsText;
    UPROPERTY()
    FString DeathsText;
    UPROPERTY()
    FString TeamText;
    UPROPERTY()
    bool PlayerIndicatorVisibility;
    UPROPERTY()
    FLinearColor TeamColor;
};
USTRUCT(BlueprintType)
struct FPlayerStats
{

    GENERATED_USTRUCT_BODY()
    /*FPlayerStats() {};
    FPlayerStats(ASTUPlayerState *PlayerState)
    {
        PlayerName = PlayerState->GetPlayerName();
        Kills = PlayerState->GetKillsNum();
        Deaths = PlayerState->GetDeathsNum();
        TeamID = PlayerState->GetTeamID();
        TeamColor = PlayerState->GetTeamColor();
    }*/
    UPROPERTY()
    FString PlayerName;
    UPROPERTY()
    int32 Kills;
    UPROPERTY()
    int32 Deaths;
    UPROPERTY()
    int32 TeamID;
    UPROPERTY()
    FLinearColor TeamColor;
};

USTRUCT(BlueprintType)
struct FMatchStatistics
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY()
    TArray<FPlayerStats> Stats;
};

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName = NAME_None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelDisplayName = NAME_None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D *LevelThumb;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData &)

    extern const FName NAME_STUSpectating;

UENUM(BlueprintType)
enum class STUPlayerStateEnum : uint8
{
    None = 0,
    Gaming,
    Spectating
};