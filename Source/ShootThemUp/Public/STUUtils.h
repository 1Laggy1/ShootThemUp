#pragma once
#include "Player/STUPlayerState.h"

#include "Components/PrimitiveComponent.h"
#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.h"

class STUUtils
{
  public:
    template <typename T> static T *GetSTUPlayerComponent(AActor *PlayerPawn)
    {
        if (!PlayerPawn)
            return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
    bool static AreEnemies(AController *Controller1, AController *Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2)
            return false;

        const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
        const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

        return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
    }
    int32 static GetRandomIDRange()
    {
        return FMath::RandRange(0, 99999);
    }
    AController static *GetInstigatorControllerFromDamageCauser(AActor *DamageCauser)
    {
        if (!DamageCauser)
            return nullptr;

        if (APawn *Pawn = Cast<APawn>(DamageCauser))
        {
            return Pawn->GetController();
        }

        if (AController *InstCtrl = DamageCauser->GetInstigatorController())
        {
            return InstCtrl;
        }

        if (AActor *OwnerCauser = DamageCauser->GetOwner())
        {
            if (APawn *OwnerPawn = Cast<APawn>(OwnerCauser))
                return OwnerPawn->GetController();

            if (AController *OwnerCtrl = OwnerCauser->GetInstigatorController())
                return OwnerCtrl;
        }

        return nullptr;
    }
    /* FPlayerInfo static GetPlayerStatsFromPlayerState(ASTUPlayerState* PlayerState)
     {
         FPlayerInfo NewPlayerStats;
         if (!PlayerState)
             return NewPlayerStats;
         NewPlayerStats.PlayerName = PlayerState->GetPlayerName();
         NewPlayerStats.Kills = PlayerState->GetKillsNum();
         NewPlayerStats.Deaths = PlayerState->GetDeathsNum();
         NewPlayerStats.TeamID = PlayerState->GetTeamID();
         NewPlayerStats.TeamColor = PlayerState->GetTeamColor();
         return NewPlayerStats;
     }*/
    FStatRowInfo static GetStatRowInfoFromFPlayerStats(FPlayerInfo &PlayerInfo)
    {
        FStatRowInfo RowInfo;
        RowInfo.PlayerNameText = PlayerInfo.PlayerName;
        RowInfo.KillsText = FString::FromInt(PlayerInfo.Kills);
        RowInfo.DeathsText = FString::FromInt(PlayerInfo.Deaths);
        RowInfo.TeamText = FString::FromInt(PlayerInfo.TeamID);
        RowInfo.TeamColor = PlayerInfo.Color;
        return RowInfo;
    }
    FTeamInfo static *FindTeamByTeamID(int32 TeamID, TArray<FTeamInfo> &Teams)
    {
        return Teams.FindByPredicate([TeamID](const FTeamInfo &Team) { return Team.TeamID == TeamID; });
    }

    static FPlayerInfo *FindPlayerByPlayerID(const FString &PlayerID, TArray<FTeamInfo> &Teams)
    {
        if (Teams.IsEmpty())
            return nullptr;
        for (FTeamInfo &Team : Teams)
        {
            FPlayerInfo *Player =
                Team.PlayersInfos.FindByPredicate([&](const FPlayerInfo &P) { return P.PlayerID == PlayerID; });
            if (Player)
            {
                return Player;
            }
        }
        return nullptr;
    }

    static void ApplyRadialDamageWithLineOfSight(UWorld *World, const FVector &Origin, const FExplosionParams &Params,
                                                 AActor *DamageCauser, AController *InstigatorController,
                                                 const TArray<AActor *> &IgnoredActors)
    {
        if (!World)
            return;

        TArray<FOverlapResult> Overlaps;
        TArray<AActor *> FinishedActors;
        FCollisionShape Sphere = FCollisionShape::MakeSphere(Params.DamageRadius);

        if (World->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_PhysicsBody, Sphere))
        {
            for (auto &Result : Overlaps)
            {
                UPrimitiveComponent *OverlappedComp = Result.GetComponent();
                AActor *OverlappedActor = Result.GetActor();
                if (!OverlappedActor || FinishedActors.Contains(OverlappedActor))
                    continue;

                // Line of sight check
                FHitResult HitResult;
                FCollisionQueryParams QueryParams;
                QueryParams.AddIgnoredActor(DamageCauser);
                QueryParams.AddIgnoredActor(OverlappedActor);
                for (AActor *Actor : IgnoredActors)
                {
                    QueryParams.AddIgnoredActor(Actor);
                }

                bool bHit = World->LineTraceSingleByChannel(HitResult, Origin, OverlappedActor->GetActorLocation(),
                                                            ECC_Visibility, QueryParams);
                if (bHit)
                    continue;

                if (ACharacter *HitCharacter = Cast<ACharacter>(OverlappedActor))
                {
                    FVector Direction = HitCharacter->GetActorLocation() - Origin;
                    float Damage = FMath::GetMappedRangeValueClamped(
                        FVector2D(0.f, Params.DamageRadius), FVector2D(Params.DamageAmount, Params.DamageAmountMin),
                        Direction.Size());
                    FPointDamageEvent DEvent;
                    DEvent.DamageTypeClass = UDamageType::StaticClass();
                    HitCharacter->TakeDamage(Damage, DEvent, InstigatorController, DamageCauser);

                    float ExplosionStrengthMapped = FMath::GetMappedRangeValueClamped(
                        FVector2D(0.f, Params.DamageRadius),
                        FVector2D(Params.ExplosionStrength, Params.ExplosionStrengthMin), Direction.Size());

                    Direction.Normalize();
                    FVector LaunchVelocity = Direction * ExplosionStrengthMapped;
                    LaunchVelocity.Z += ExplosionStrengthMapped / 3;
                    HitCharacter->LaunchCharacter(LaunchVelocity, true, true);
                }
                else if (OverlappedComp && OverlappedComp->IsSimulatingPhysics())
                {
                    OverlappedComp->AddRadialImpulse(Origin, Params.DamageRadius, Params.ExplosionStrength,
                                                     ERadialImpulseFalloff::RIF_Linear, true);
                }
                FinishedActors.Add(OverlappedActor);
            }
        }
    }
};
