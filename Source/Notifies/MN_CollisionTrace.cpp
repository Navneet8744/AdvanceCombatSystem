


#include "Notifies/MN_CollisionTrace.h"
#include "Interfaces/I_DamageAble.h"

void UMN_CollisionTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerClass = MeshComp->GetOwner();

	if (IsValid(OwnerClass) && OwnerClass->Implements<UI_DamageAble>())
	{

		II_DamageAble::Execute_CollisionTrace_Start(OwnerClass);
	}

}

void UMN_CollisionTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);


	AActor* OwnerClass = MeshComp->GetOwner();

	if (IsValid(OwnerClass) && OwnerClass->Implements<UI_DamageAble>())
	{
		
		II_DamageAble::Execute_CollisionTrace_End(OwnerClass);
	}

}
