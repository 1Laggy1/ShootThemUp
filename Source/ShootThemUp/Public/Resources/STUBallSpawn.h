// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resources/STUBall.h"
#include "STUBallSpawn.generated.h"
class UWidgetComponent;
class USTUBallSpawnWidget;
UCLASS()
class SHOOTTHEMUP_API ASTUBallSpawn : public AActor
{
    GENERATED_BODY()

  public:

    ASTUBallSpawn();
    float GetTimeLeftToSpawn()
    {
        return TimeLeftToSpawn;
    }
    float GetTimeToSpawnStart()
    {
        return TimeToSpawnStart;
    }
    int32 GetTeamID() const
    {
        return TeamID;
    }
    void StartSpawning(float TimeToSpawn);
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
  protected:
    UPROPERTY(EditDefaultsOnly, Category = "STU")
    TSubclassOf<ASTUBall> BallClass;
    virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UWidgetComponent *SpawnWidgetComponent;
    UFUNCTION(NetMulticast, Reliable)
    void SetVisible(bool Visible);
  private:
    void SpawnBall();
    void Tick(float DeltaTime) override;
    void SpawningTimerUpdate();
    UPROPERTY(Replicated)
    float TimeLeftToSpawn = 0;
    UPROPERTY(Replicated)
    float TimeToSpawnStart = 0;
    UPROPERTY(EditAnywhere, Category = "Team")
    int32 TeamID = 0;
    FTimerHandle SpawnTimerHandle;
    USTUBallSpawnWidget *SpawnWidget;
    
};
