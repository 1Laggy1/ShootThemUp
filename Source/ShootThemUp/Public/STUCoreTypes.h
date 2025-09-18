#pragma once
#include "STUCoreTypes.generated.h"
// weapon
class ASTUBaseWeapon;
class USoundCue;
class ASTUTeamPlayerStart;
class ASTUBallSpawn;
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

//USTRUCT(BlueprintType)
//struct FWeaponData
//{
//    GENERATED_USTRUCT_BODY()
//    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
//    TSubclassOf<ASTUBaseWeapon> WeaponClass;
//
//    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
//    UAnimMontage *ReloadAnimMontage;
//};

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
struct FPlayerInfo
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnInfo")
    FLinearColor Color = FLinearColor::White;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnInfo")
    TSoftClassPtr<ASTUBaseWeapon> WeaponClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnInfo")
    FString PlayerName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnInfo")
    FString PlayerID;
    UPROPERTY()
    int32 TeamID;
    UPROPERTY()
    int32 Kills;
    UPROPERTY()
    int32 Deaths;
    APlayerController *ThisPlayerController;

};

USTRUCT(BlueprintType)
struct FTeamInfo
{
    GENERATED_USTRUCT_BODY()

    FTeamInfo() : TeamID(1), TeamColor(FLinearColor::White), TeamName("Team 1")
    {
    }

    FTeamInfo(int32 ID, const FLinearColor &Color = FLinearColor::White, const FString &Name = FString())
        : TeamID(ID), TeamColor(Color), TeamName(Name.IsEmpty() ? FString("Team ") + FString::FromInt(ID) : Name)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TeamID = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor TeamColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TeamName = "Team 1";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPlayerInfo> PlayersInfos;
    UPROPERTY()
    int32 Score;
    UPROPERTY()
    TArray<ASTUTeamPlayerStart *> TeamStarts;
    UPROPERTY()
    ASTUBallSpawn *BallSpawnPoint = nullptr;
};

//USTRUCT(BlueprintType)
//struct FMatchStatistics
//{
//    GENERATED_USTRUCT_BODY()
//    UPROPERTY()
//    TArray<FPlayerStats> Stats;
//};

USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName DisplayName = NAME_None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D *Thumb;
};

USTRUCT(BlueprintType)
struct  FLevelData : public FItemData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName = NAME_None;
};
USTRUCT(BlueprintType)
struct FWeaponItemData : public FItemData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData &)
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponSelectedSignature,
                                        const FWeaponItemData &)
    extern const FName NAME_STUSpectating;

UENUM(BlueprintType)
enum class STUPlayerStateEnum : uint8
{
    None = 0,
    Gaming,
    Spectating
};

