#include "AI/BTTask/BTS_SetEnemyState.h"
#include "AIController.h"
#include "Enemy/Enemy.h"



UBTS_SetEnemyState::UBTS_SetEnemyState()
{
	NodeName = "Set Enemy State";
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = false;
	bNotifyTick = false;
}

void UBTS_SetEnemyState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();

	if (!Controller) return;

	if (AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn()))
	{

		Enemy->SetEnemyState(EnemyState);

	}
}
