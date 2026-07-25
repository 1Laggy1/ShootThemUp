// Shoot THem Up Game. All Rights Reserved.


#include "Weapon/STURailgunWeapon.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/STUPlayerState.h"
#include "Components/AudioComponent.h"

#include "GameFramework/Character.h"

#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY_STATIC(LogRailgunWeapon, All, All);

ASTURailgunWeapon::ASTURailgunWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    ChargeNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargeNiagaraComponent"));
    ChargeNiagaraComponent->SetupAttachment(WeaponMesh, "MuzzleFlashSocket");
    ChargeNiagaraComponent->bAutoActivate = false;
    ChargeAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ChargeAudioComponent"));
    ChargeAudioComponent->SetupAttachment(WeaponMesh);
    ChargeAudioComponent->bAutoActivate = false;
}

void ASTURailgunWeapon::StartFire()
{
    if (isClipEmpty())
    {
        StopFire();
        if (NoAmmoSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        }

        return;
    }
    StartCharge();
    StartCharge_Server();
}

void ASTURailgunWeapon::StopFire()
{
    Discharge();
    StopCharge_Server();

}

void ASTURailgunWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsCharging)
    {
        Charge += DeltaTime;
        ChargePercent = FMath::Clamp(Charge / TimeToCharge, 0.0f, 1.0f);
    }
    else if (Charge > 0 && !bIsCharging)
    {
        Charge -= DeltaTime;
        ChargePercent = FMath::Clamp(Charge / TimeToCharge, 0.0f, 1.0f);
    }
    if (ChargePercentPrevious != ChargePercent)
    {
       
        ChargeNiagaraComponent->SetFloatParameter(FName("User.ChargeProgress"), ChargePercent * ChargeScaleMultiplier);
        ChargePercentPrevious = ChargePercent;
       
    }
    if (Charge >= TimeToCharge)
    {
        ChargeComplete();
        APawn *PawnOwner = Cast<APawn>(GetOwner());
        if (PawnOwner && PawnOwner->IsLocallyControlled())
        {
            MakeShot();
        }
    }
    
        
}

void ASTURailgunWeapon::StartCharge()
{
    if (ChargeNiagaraComponent)
        if (!ChargeNiagaraComponent->IsActive())
        {
            ChargeNiagaraComponent->Activate();
        }
    if (ChargeAudioComponent)
    {
        ChargeAudioComponent->Stop();
        ChargeAudioComponent->SetSound(ChargeSound);
        ChargeAudioComponent->Play();
    }
    bIsCharging = true;
}

void ASTURailgunWeapon::Discharge()
{
    if (!bIsCharging)
        return;
    if (ChargeAudioComponent)
    {
        ChargeAudioComponent->Stop();
        ChargeAudioComponent->SetSound(DischargeSound);
        ChargeAudioComponent->Play();
    }

    bIsCharging = false;
}

void ASTURailgunWeapon::ChargeComplete()
{
    if (ChargeAudioComponent)
    {
        ChargeAudioComponent->Stop();
        ChargeAudioComponent->SetSound(DischargeSound);
        ChargeAudioComponent->Play();
    }
    if (ChargeNiagaraComponent)
    {
        ChargeNiagaraComponent->DeactivateImmediate();
    }
    Charge = 0.0f;
    ChargePercent = 0.0f;
    ChargePercentPrevious = 0.0f;
    
    bIsCharging = false;
}

void ASTURailgunWeapon::StartCharge_Server_Implementation()
{
    StartCharge_Multicast();
}

void ASTURailgunWeapon::StartCharge_Multicast_Implementation()
{
    APawn *PawnOwner = Cast<APawn>(GetOwner());
    
    if (!PawnOwner || !PawnOwner->IsLocallyControlled())
    {
        StartCharge();
    }
}

void ASTURailgunWeapon::StopCharge_Server_Implementation()
{
    StopCharge_Multicast();
}

void ASTURailgunWeapon::StopCharge_Multicast_Implementation()
{
    APawn *PawnOwner = Cast<APawn>(GetOwner());
    if (!PawnOwner || !PawnOwner->IsLocallyControlled())
    {
        Discharge();
    }
}