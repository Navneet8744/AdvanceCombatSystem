


#include "AI/BTTask/BTT_SetMovementState.h"
#include "AIController.h"
#include "Enemy/Enemy.h"

UBTT_SetMovementState::UBTT_SetMovementState()
{
	NodeName = "Set Movement Speed";


}

EBTNodeResult::Type UBTT_SetMovementState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();

	if (!Controller) return EBTNodeResult::Failed;

	APawn* Pawn = Controller->GetPawn();


	if (!Pawn) return EBTNodeResult::Failed;





	AEnemy* Enemy = Cast<AEnemy>(Pawn);

	if (!Enemy) return EBTNodeResult::Failed;

	if (Enemy)
	{
		if (Enemy->Implements<UI_EnemyAI>())
		{
			II_EnemyAI::Execute_SetMovementState(Enemy, NewState);
			UE_LOG(LogTemp, Warning, TEXT("Movement is Set"));
			return EBTNodeResult::Succeeded;
		}

	}

	return EBTNodeResult::Failed;


}


	
