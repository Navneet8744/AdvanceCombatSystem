// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTT_MoveToPatrolPoint.generated.h"

struct FBTMoveAlongPatrolMemory
{
	TWeakObjectPtr<class AAIController> CachedAIOwner;
	FAIRequestID MoveRequestID;
	FDelegateHandle MoveDelegateHandle;
};


UCLASS()
class COMBATDAMAGE_API UUBTT_MoveToPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()


public:

	UUBTT_MoveToPatrolPoint();

	virtual uint16  GetInstanceMemorySize() const override { return sizeof(FBTMoveAlongPatrolMemory); }

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:

	void OnMoveCompleted(FAIRequestID RequestID, const struct FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp);

	UPROPERTY(EditAnywhere,Category = "Patrol")
	float AcceptanceRadius = 50.f;


	UPROPERTY(EditAnywhere,Category = "Patrol")
	bool bStopOnOverlap = true;
};
