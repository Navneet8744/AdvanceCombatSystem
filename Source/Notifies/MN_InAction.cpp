


#include "Notifies/MN_InAction.h"
#include "Interfaces/I_DamageAble.h"

void UMN_InAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* ownerClass = MeshComp->GetOwner();

	if ( IsValid(ownerClass) && ownerClass->Implements<UI_DamageAble>())
	{
		II_DamageAble::Execute_SetActionState(ownerClass, EActionState::EAS_Engaged);
	}

}

void UMN_InAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* ownerClass = MeshComp->GetOwner();

	if (IsValid(ownerClass) && ownerClass->Implements<UI_DamageAble>())
	{
		II_DamageAble::Execute_SetActionState(ownerClass, EActionState::EAS_UnOccupied);
	}
}
