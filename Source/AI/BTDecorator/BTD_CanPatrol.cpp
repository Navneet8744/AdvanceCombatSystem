// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTD_CanPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CanPatrol::UBTD_CanPatrol()
{
	NodeName = "Can Patrol";


	bCreateNodeInstance = true;
	
}

bool UBTD_CanPatrol::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp) return false;

	const UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);

	const AActor* TargetActor = Cast<AActor>(TargetObject);

	const FVector TargetLocation = BlackboardComp->GetValueAsVector(TargetLocationKey.SelectedKeyName);

	if (!IsValid(TargetActor) && TargetLocation == FAISystem::InvalidLocation)
	{
		return true;
	}

	return false;
}
