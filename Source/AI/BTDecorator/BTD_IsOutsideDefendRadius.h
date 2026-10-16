// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_IsOutsideDefendRadius.generated.h"


UCLASS()
class COMBATDAMAGE_API UBTD_IsOutsideDefendRadius : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector  DefendRadius;


	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackTargetKey;

	UBTD_IsOutsideDefendRadius();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
