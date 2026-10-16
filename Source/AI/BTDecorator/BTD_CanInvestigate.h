// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_CanInvestigate.generated.h"


UCLASS()
class COMBATDAMAGE_API UBTD_CanInvestigate : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()


public:


	UBTD_CanInvestigate();

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InvestigateLocationKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DamageLocationKey;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const  override;
	
};
