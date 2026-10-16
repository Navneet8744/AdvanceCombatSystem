// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/UBTT_MoveToPatrolPoint.h"
#include "AIController.h"
#include "Enemy/Enemy.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/PatrolPoint/PatrolPoints.h"


UUBTT_MoveToPatrolPoint::UUBTT_MoveToPatrolPoint()
{
	NodeName = "Move Along Patrol Point ";

	bCreateNodeInstance = true;

	bNotifyTaskFinished = true;
	
}

EBTNodeResult::Type UUBTT_MoveToPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("========== PATROL MOVE TASK STARTED =========="));
	FBTMoveAlongPatrolMemory* MyMemory = reinterpret_cast<FBTMoveAlongPatrolMemory*>(NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());


	if (!Enemy) return EBTNodeResult::Failed;

	if (!Enemy->Implements<UI_EnemyAI>()) return EBTNodeResult::Failed;

	APatrolPoints* PatrolPoint = II_EnemyAI::Execute_GetPatrolPoint(Enemy);

	if (!PatrolPoint) return EBTNodeResult::Failed;


	const FVector GoalLocation = PatrolPoint->GetSplinePointAsWorldPosition();

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(GoalLocation);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAllowPartialPath(true);


	FPathFollowingRequestResult  RequestResult = AIController->MoveTo(MoveRequest);

	UE_LOG(LogTemp,Warning,TEXT("MoveTo Result: %d | MoveID: %d | Goal: %s"),static_cast<int32>(RequestResult.Code),RequestResult.MoveId.GetID(),*GoalLocation.ToString());

	if (RequestResult.Code == EPathFollowingRequestResult::Failed) return EBTNodeResult::Failed;



	if (RequestResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		PatrolPoint->IncrementPatrolPoint();
		return EBTNodeResult::Succeeded;
	}
	
  
	MyMemory->CachedAIOwner = AIController;
	MyMemory->MoveRequestID = RequestResult.MoveId;

	UPathFollowingComponent* PathComp = AIController->GetPathFollowingComponent();
	
	if (!PathComp) return EBTNodeResult::Failed;

	MyMemory->MoveDelegateHandle = PathComp->OnRequestFinished.AddUObject(this, &UUBTT_MoveToPatrolPoint::OnMoveCompleted, &OwnerComp);

	return EBTNodeResult::InProgress;


}



void UUBTT_MoveToPatrolPoint::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp)
{
	if (!OwnerComp) return;

	uint8* NodeMemory = OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this));
	FBTMoveAlongPatrolMemory* MyMemory = reinterpret_cast<FBTMoveAlongPatrolMemory*>(NodeMemory);


	if (MyMemory->MoveRequestID != RequestID) return;


	if (MyMemory->CachedAIOwner.IsValid())
	{
		if (UPathFollowingComponent* PathComp = MyMemory->CachedAIOwner->GetPathFollowingComponent())
		{
			PathComp->OnRequestFinished.Remove(MyMemory->MoveDelegateHandle);
		}

	}

	if (Result.Code == EPathFollowingResult::Success)
	{
		if (MyMemory->CachedAIOwner.IsValid())
		{
			if (AEnemy* Enemy = Cast<AEnemy>(MyMemory->CachedAIOwner->GetPawn()))
			{
				if (Enemy->Implements<UI_EnemyAI>())
				{
					if (APatrolPoints* PatrolPoint = II_EnemyAI::Execute_GetPatrolPoint(Enemy))
					{
						PatrolPoint->IncrementPatrolPoint();
					}
				}
			}
		}

		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}

	else if (Result.Code == EPathFollowingResult::Aborted)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Aborted);
	}
	else
	{
		
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	}
}



EBTNodeResult::Type UUBTT_MoveToPatrolPoint::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTMoveAlongPatrolMemory* MyMemory = reinterpret_cast<FBTMoveAlongPatrolMemory*>(NodeMemory);

	if (MyMemory->CachedAIOwner.IsValid())
	{
		if (UPathFollowingComponent* PathComp = MyMemory->CachedAIOwner->GetPathFollowingComponent())
		{
			PathComp->OnRequestFinished.Remove(MyMemory->MoveDelegateHandle);
		}

		MyMemory->CachedAIOwner->StopMovement();
	}

	return EBTNodeResult::Aborted;

}



