// Shoot THem Up Game. All Rights Reserved.


#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation,
                                         const FAnimNotifyEventReference &EventReference)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation, EventReference);
}
