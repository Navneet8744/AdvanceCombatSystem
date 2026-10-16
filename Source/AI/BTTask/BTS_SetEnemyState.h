// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ENUM/ENUM.h"
#include "BTS_SetEnemyState.generated.h"


UCLASS()
class COMBATDAMAGE_API UBTS_SetEnemyState : public UBTService
{
	GENERATED_BODY()


public:

	UBTS_SetEnemyState();
	

protected:

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "State")
	EEnemyState EnemyState;
	
};
