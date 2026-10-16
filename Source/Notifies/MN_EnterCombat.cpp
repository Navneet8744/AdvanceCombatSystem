


#include "Notifies/MN_EnterCombat.h"
#include "Interfaces/I_DamageAble.h"



void UMN_EnterCombat::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* ownerClass = MeshComp->GetOwner();

	if (IsValid(ownerClass) && ownerClass->Implements<UI_DamageAble>())
	{
		
		II_DamageAble::Execute_EnterCombat(ownerClass);
		II_DamageAble::Execute_SetActionState(ownerClass, EActionState::EAS_Engaged);
	}
	
}




void UMN_EnterCombat::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* ownerClass = MeshComp->GetOwner();

	if (IsValid(ownerClass) && ownerClass->Implements<UI_DamageAble>())
	{
		II_DamageAble::Execute_SetActionState(ownerClass, EActionState::EAS_UnOccupied);
	}


}
