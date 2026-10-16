// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidgetClass.generated.h"


UCLASS()
class COMBATDAMAGE_API UEnemyWidgetClass : public UUserWidget
{
	GENERATED_BODY()



public:

	UFUNCTION(BlueprintImplementableEvent)
	void SetHealthPercent(float Percent);

	
};
