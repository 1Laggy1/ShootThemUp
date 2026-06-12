// Shoot THem Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUAnimNotify.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FNotifySignature, USkeletalMeshComponent *);
UCLASS()
class SHOOTTHEMUP_API USTUAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
  public:
    void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation,
                const FAnimNotifyEventReference &EventReference) override;
    FNotifySignature OnNotified;
};
