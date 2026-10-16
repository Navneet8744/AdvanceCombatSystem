// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_HasPatrolPoints.generated.h"


UCLASS()
class COMBATDAMAGE_API UBTD_HasPatrolPoints : public UBTDecorator
{
	GENERATED_BODY()

public:


	UBTD_HasPatrolPoints();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
