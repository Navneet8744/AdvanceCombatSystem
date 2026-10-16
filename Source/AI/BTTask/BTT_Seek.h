// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BTT_Seek.generated.h"


UCLASS()
class COMBATDAMAGE_API UBTT_Seek : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_Seek();

		
	UPROPERTY(EditAnywhere)
	UEnvQuery* SeekQuery;

	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	TWeakObjectPtr<AAIController> CachedController = nullptr;



protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	
	void OnSeekQueryFinished(TSharedPtr<FEnvQueryResult> Results);
	
};
