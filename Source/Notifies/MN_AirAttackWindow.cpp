


#include "Notifies/MN_AirAttackWindow.h"
#include "Interfaces/I_EnemyAI.h"




//void UMN_AirAttackWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
//{
//	AActor* Actor = Cast<AActor>(MeshComp->GetOwner());
//
//	if (!Actor) return;
//
//	if (Actor->Implements<UI_EnemyAI>())
//	{
//		II_EnemyAI::Execute_SetAirState(Actor,true);
//	}
//}
//
//void UMN_AirAttackWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
//{
//	AActor* Actor = Cast<AActor>(MeshComp->GetOwner());
//	if (!Actor) return;
//
//	if (Actor->Implements<UI_EnemyAI>())
//	{
//		II_EnemyAI::Execute_SetAirState(Actor, false);
//	}
//}
