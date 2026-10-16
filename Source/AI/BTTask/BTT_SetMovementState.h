// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Enum/ENUM.h"
#include "BTT_SetMovementState.generated.h"

/**
 * 
 */
UCLASS()
class COMBATDAMAGE_API UBTT_SetMovementState : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_SetMovementState();


	UPROPERTY(EditAnywhere)
	EMovementState NewState = EMovementState::EMS_Idle;


protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
