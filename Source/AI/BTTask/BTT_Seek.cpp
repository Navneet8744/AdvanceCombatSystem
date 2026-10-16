


#include "AI/BTTask/BTT_Seek.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enum/ENUM.h"
#include "AIController.h"


UBTT_Seek::UBTT_Seek()
{
	NodeName = "Seek Target";
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Seek::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	CachedController = OwnerComp.GetAIOwner();	


	if (!CachedOwnerComp.Get()) return EBTNodeResult::Failed;
	if (!SeekQuery)
	{
		return  EBTNodeResult::Failed;
	}

	AActor* Actor = Cast<AActor>(CachedController->GetPawn());

	if (!Actor) return EBTNodeResult::Failed;

	FEnvQueryRequest Request(SeekQuery, Actor);

	Request.Execute(EEnvQueryRunMode::SingleResult, this, &UBTT_Seek::OnSeekQueryFinished);

	return EBTNodeResult::InProgress;
}

void UBTT_Seek::OnSeekQueryFinished(TSharedPtr<FEnvQueryResult> Results)
{
	if (!CachedOwnerComp.IsValid()) return;


	if (!Results.IsValid() || !Results->IsSuccessful())
	{
		
			CachedOwnerComp->GetBlackboardComponent()->ClearValue("LastSeenLocation");
			CachedOwnerComp->GetBlackboardComponent()->ClearValue("SeekLocation");
			FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Failed);
			return;
		
		
	}




	FVector SeekingLocation = Results->GetItemAsLocation(0);
	CachedOwnerComp->GetBlackboardComponent()->SetValueAsVector("SeekLocation", SeekingLocation);

	FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Succeeded);

}
