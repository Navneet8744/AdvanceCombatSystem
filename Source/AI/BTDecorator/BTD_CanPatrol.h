// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_CanPatrol.generated.h"

/**
 * 
 */
UCLASS()
class COMBATDAMAGE_API UBTD_CanPatrol : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTD_CanPatrol();

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetLocationKey;


	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
};
