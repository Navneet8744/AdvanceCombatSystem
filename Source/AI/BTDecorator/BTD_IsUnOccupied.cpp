// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTD_IsUnOccupied.h"
#include "AIController.h"
#include "ENUM/Enum.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsUnOccupied::UBTD_IsUnOccupied()
{
	NodeName = "Is UnOccupied";

	bCreateNodeInstance = true;


}

bool UBTD_IsUnOccupied::CalculateRawConditionValue(
    UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory) const
{
    AAIController* Controller = OwnerComp.GetAIOwner();

    if (!Controller)
    {
        return false;
    }

    const UBlackboardComponent* Blackboard =
        Controller->GetBlackboardComponent();

    if (!Blackboard)
    {
        return false;
    }

    const EActionState ActionState =
        static_cast<EActionState>(
            Blackboard->GetValueAsEnum(GetSelectedBlackboardKey())
            );

    return ActionState != EActionState::EAS_Engaged;
}
