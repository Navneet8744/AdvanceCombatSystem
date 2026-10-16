


#include "AI/BTTask/BTT_ClearFocu.h"
#include "AIController.h"





UBTT_ClearFocu::UBTT_ClearFocu()
{

	NodeName = " Clear Focus";


}






EBTNodeResult::Type UBTT_ClearFocu::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* Controller = OwnerComp.GetAIOwner();

	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	Controller->ClearFocus(EAIFocusPriority::Gameplay);

	return EBTNodeResult::Succeeded;

}
