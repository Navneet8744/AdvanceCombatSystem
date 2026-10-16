
#include "AI/BTDecorator/BTD_HasPatrolPoints.h"
#include "AIController.h"
#include "Enemy/Enemy.h"
#include "Interfaces/I_EnemyAI.h"

UBTD_HasPatrolPoints::UBTD_HasPatrolPoints()
{
	NodeName = "Has Patrol Point";

	bNotifyBecomeRelevant = false;
	bNotifyCeaseRelevant = false;

}

bool UBTD_HasPatrolPoints::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();

	if (!Controller) return false;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn());


	if (!Enemy) return false;

	if (Enemy->Implements<UI_EnemyAI>())
	{
		if (II_EnemyAI::Execute_GetPatrolPoint(Enemy))
		{
			return true;
		}
	}
	
	return false;
}

