

#include "AI/BTDecorator/BTD_CanSeek.h"

#include "BehaviorTree/BlackboardComponent.h"



UBTD_CanSeek::UBTD_CanSeek()
{

	NodeName = "Can Seek";

	bCreateNodeInstance = false;




}

bool UBTD_CanSeek::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{


	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	

	if (!BlackboardComp || !BlackboardComp->IsVectorValueSet(SeekTarget.SelectedKeyName))
	{
		return false;
	}

	const FVector SeekLocation = BlackboardComp->GetValueAsVector(SeekTarget.SelectedKeyName);
	return !SeekLocation.ContainsNaN() && (SeekLocation != FAISystem::InvalidLocation);
}

