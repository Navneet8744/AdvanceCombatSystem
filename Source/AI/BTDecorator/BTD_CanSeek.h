
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_CanSeek.generated.h"

/**
 * 
 */
UCLASS()
class COMBATDAMAGE_API UBTD_CanSeek : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()


public:


	UBTD_CanSeek();


	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SeekTarget;
	
};
