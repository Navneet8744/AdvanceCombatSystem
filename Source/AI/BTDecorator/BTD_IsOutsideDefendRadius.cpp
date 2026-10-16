


#include "AI/BTDecorator/BTD_IsOutsideDefendRadius.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsOutsideDefendRadius::UBTD_IsOutsideDefendRadius()
{
	NodeName = "IsOutsideDefendRadius";

	bCreateNodeInstance = true;


}

bool UBTD_IsOutsideDefendRadius::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();

	if (!Controller) return false;

	AActor* Owner = Cast<AActor>(Controller->GetPawn());

	if (!Owner) return false;

	UBlackboardComponent* BBC = Controller->GetBlackboardComponent();

	if (!BBC) return false;


	AActor* AttackTarget = Cast<AActor>(BBC->GetValueAsObject(AttackTargetKey.SelectedKeyName));

	if (!AttackTarget) return false;	

	float DefendRange =  OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DefendRadius.SelectedKeyName);

	if (DefendRange <= 0.f) return false;

	float DistanceFromTarget = Owner->GetDistanceTo(AttackTarget);

	float  Tolerance = 50.f;

	return DistanceFromTarget >= (DefendRange + Tolerance);


  	
}


