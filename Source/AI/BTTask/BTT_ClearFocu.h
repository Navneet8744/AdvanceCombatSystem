// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ClearFocu.generated.h"


UCLASS()
class COMBATDAMAGE_API UBTT_ClearFocu : public UBTTaskNode
{
	GENERATED_BODY()

public:


	UBTT_ClearFocu();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	

	
};