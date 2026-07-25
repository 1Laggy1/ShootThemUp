// Shoot THem Up Game. All Rights Reserved.


#include "UI/STUBallSpawnWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Resources/STUBallSpawn.h"



void USTUBallSpawnWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!BallSpawnInstance || BallSpawnInstance->GetTimeToSpawnStart() == 0)
        return;

    if (SpawnProgressBar && SpawnText)
    {
        float TimeToShow = BallSpawnInstance->GetTimeLeftToSpawn() /
                           BallSpawnInstance->GetTimeToSpawnStart();
        SpawnProgressBar->SetPercent(FMath::Clamp(1-TimeToShow, 0.f, 1.f));
        SpawnText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), BallSpawnInstance->GetTimeLeftToSpawn())));
    }
}


void USTUBallSpawnWidget::SetVisible(bool isActive)
{
    if (SpawnProgressBar && SpawnText)
    {
        ESlateVisibility Visible = isActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
        SpawnProgressBar->SetVisibility(Visible);
        SpawnText->SetVisibility(Visible);
    }
}