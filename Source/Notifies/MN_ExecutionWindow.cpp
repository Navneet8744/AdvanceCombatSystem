


#include "Notifies/MN_ExecutionWindow.h"
#include "GameFramework/Character.h"
#include "Interfaces/I_EnemyAI.h"

void UMN_ExecutionWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* ownerClass = MeshComp->GetOwner();

	if (IsValid(ownerClass) && ownerClass->Implements<UI_EnemyAI>())
	{
		II_EnemyAI::Execute_SetExecution(ownerClass,true);
	}


}





void UMN_ExecutionWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* ownerClass = MeshComp->GetOwner();

	if (IsValid(ownerClass) && ownerClass->Implements<UI_EnemyAI>())
	{
		II_EnemyAI::Execute_SetExecution(ownerClass, false);
	}

}
