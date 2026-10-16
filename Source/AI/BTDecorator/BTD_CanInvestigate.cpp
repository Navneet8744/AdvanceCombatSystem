#include "AI/BTDecorator/BTD_CanInvestigate.h"
#include "BehaviorTree/BlackboardComponent.h"




UBTD_CanInvestigate::UBTD_CanInvestigate()
{
	NodeName = "Can Investigate";
	bCreateNodeInstance = true;
}




bool UBTD_CanInvestigate::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	if (!BBC) return false;

	FVector  DamageLocation = BBC->GetValueAsVector(DamageLocationKey.SelectedKeyName);
	FVector  InvestigateLocation = BBC->GetValueAsVector(InvestigateLocationKey.SelectedKeyName);

	if (DamageLocation != FAISystem::InvalidLocation || InvestigateLocation != FAISystem::InvalidLocation)
	{
		if (DamageLocation != FAISystem::InvalidLocation)
		{
			return true;
		}

		if (InvestigateLocation != FAISystem::InvalidLocation)
		{
			return true;
		}
	}
	return false;
		 

}
