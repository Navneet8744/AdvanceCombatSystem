// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTT_FocusTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTT_FocusTarget::UBTT_FocusTarget()
{
	NodeName = " Focus Target";

	

}



EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController) return  EBTNodeResult::Failed;


	AActor* TargetPlayer = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	FVector  TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationKey.SelectedKeyName);


	if (IsValid(TargetPlayer))
	{
		AIController->SetFocus(TargetPlayer,EAIFocusPriority::Gameplay);
		return EBTNodeResult::Succeeded;
	}



	//if(FAISystem::IsValidLocation(TargetLocation))
	//{
	//	AIController->SetFocalPoint(TargetLocation);
	//	return EBTNodeResult::Succeeded;
	//}


	return EBTNodeResult::Failed;




}



