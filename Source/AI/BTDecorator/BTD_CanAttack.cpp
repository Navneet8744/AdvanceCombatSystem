

#include "AI/BTDecorator/BTD_CanAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CanAttack::UBTD_CanAttack()
{
	NodeName = " Can Attack";

	bCreateNodeInstance  = true;
	

	
}

bool UBTD_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp) return false;

	const UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);

	const AActor* TargetActor = Cast<AActor>(TargetObject);


	if (IsValid(TargetActor))
	{
		return true;
	}

	return false;
	
}
