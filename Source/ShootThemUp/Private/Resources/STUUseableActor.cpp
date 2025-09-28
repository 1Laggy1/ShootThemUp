// Shoot THem Up Game. All Rights Reserved.


#include "Resources/STUUseableActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "UI/STUItemUseWidget.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h" 
void ASTUUseableActor::ShowItem()
{
    TimeToHideRemaining = TimeAfterHide;
    ItemWidget->SetRenderOpacity(1.0f);
	if (!IsWidgetShown)
	{
        IsWidgetShown = true;
	}
}

ASTUUseableActor::ASTUUseableActor()
{
	PrimaryActorTick.bCanEverTick = true;
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemWidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);
    SetReplicates(true);
    SetReplicateMovement(true);
}

bool ASTUUseableActor::Use(FVector Location, FVector Rotation, AController *InstigatedBy)
{
    if (CooldownRemaining > 0.0f)
        return false;
	CooldownRemaining = CooldownTime;
    GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ASTUUseableActor::CooldownTick, 0.1f, true);
    UseFX_Multicast();
    return true;
	
}
void ASTUUseableActor::UseFX_Multicast_Implementation()
{
	if (!UseSounds.IsEmpty())
	{
        for (USoundCue *Sound : UseSounds)
		{
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
		}
        
	}
	if (NiagaraEffect)
	{
        FVector WorldOffset = GetActorRotation().RotateVector(RelativeEffectPos);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, GetActorLocation() + WorldOffset,
                                                       GetActorRotation() + RelativeEffectRotation);
	}
}

void ASTUUseableActor::Tick(float DeltaTime)
{
    if (!IsWidgetShown)
        return;

	if (TimeToHideRemaining > 0.0f)
	{
        TimeToHideRemaining -= DeltaTime;
	}
	else
	{
		if (ItemWidget->GetRenderOpacity() > 0.0f)
		{
            ItemWidget->SetRenderOpacity(ItemWidget->GetRenderOpacity() - (DeltaTime * HideSpeed));
		}
		else
		{
            IsWidgetShown = false;
		}
        
	}
}

void ASTUUseableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTUUseableActor, CooldownRemaining);
}

void ASTUUseableActor::BeginPlay()
{
	Super::BeginPlay();
	if (WidgetComponent)
	{
        ItemWidget = Cast<USTUItemUseWidget>(WidgetComponent->GetUserWidgetObject());
		if (ItemWidget)
		{
            ItemWidget->SetMyItem(this);
		}
	}
}

void ASTUUseableActor::CooldownTick()
{
	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining -= 0.1f;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
    }

}

