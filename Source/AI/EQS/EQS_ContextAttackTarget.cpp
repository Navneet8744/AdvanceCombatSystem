


#include "AI/EQS/EQS_ContextAttackTarget.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "AIController.h"
#include "BehaviorTree//BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"



UEQS_ContextAttackTarget::UEQS_ContextAttackTarget()
{
}

void UEQS_ContextAttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{	
    Super::ProvideContext(QueryInstance, ContextData);

	UObject* QueryOwner = QueryInstance.Owner.Get();

	if (!QueryOwner) return;

	AAIController* Controller = Cast<AAIController>(QueryOwner);

	if (!Controller)
	{
		if (APawn* Pawn = Cast<APawn>(QueryOwner))
		{
			Controller = Cast<AAIController>(Pawn->GetController());
		}

	}

	if (!Controller) return;

	UBlackboardComponent* BBC = Controller->GetBlackboardComponent();

	if (!BBC) return;

	const FName TargetName = FName("TargetActor");

	AActor* TargetActor = Cast<AActor>(BBC->GetValueAsObject(TargetName));

	if (TargetActor)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
		return;
	}


	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	APlayerStart* PlayerStart = Cast<APlayerStart>(FoundActor);

	if (PlayerStart)
	{

		UEnvQueryItemType_Actor::SetContextHelper(ContextData, PlayerStart);
	}


}
