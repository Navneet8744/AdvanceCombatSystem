// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ENUM/ENUM.h"
#include "I_EnemyAI.generated.h"

class APatrolPoints;

UINTERFACE(MinimalAPI)
class UI_EnemyAI : public UInterface
{
	GENERATED_BODY()
};


class COMBATDAMAGE_API II_EnemyAI
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	APatrolPoints* GetPatrolPoint() const;


	UFUNCTION(BlueprintNativeEvent)
	void SetExecution(bool CanExecute);


	UFUNCTION(BlueprintNativeEvent)
	bool CanBeExecute();

	UFUNCTION(BlueprintNativeEvent)
	void SetAirDamageState(EAirDamageState DamageState);

	UFUNCTION(BlueprintNativeEvent)
	EAirDamageState GetAirDamageState();


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetMovementState(EMovementState MovementState);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EEnemyState GetEnemyState() const;

};
