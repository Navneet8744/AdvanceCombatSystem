

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQS_ContextAttackTarget.generated.h"


UCLASS(Blueprintable)
class COMBATDAMAGE_API UEQS_ContextAttackTarget : public UEnvQueryContext
{
	GENERATED_BODY()


public:

	UEQS_ContextAttackTarget();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* ContextTarget;


protected :
	
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

	
};
 