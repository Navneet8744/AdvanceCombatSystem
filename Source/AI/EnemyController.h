

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "GenericTeamAgentinterface.h"
#include "EnemyController.generated.h"


class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;


UCLASS()
class COMBATDAMAGE_API AEnemyController : public ADetourCrowdAIController
{
	GENERATED_BODY()


public:

	AEnemyController();

	virtual void Tick(float DeltaTime) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other)const override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	static const FName TargetActorKey;
	static const FName TargetLocationKey;
	static const FName LastSeenLocationKey;
	static const FName InvestigateLocationKey;
	static const FName DamageLocationKey;
	static const FName IdealRangeKey;
	static const FName DefendRangeKey;





protected:


	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()

	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()

	void OnTargetPerceptionForgotten(AActor* Actor);


	UPROPERTY()
	FAIStimulus LastDamageStimulus;

	FAIStimulus LastHearingStimulus;


private:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

	UPROPERTY()
	FGenericTeamId TeamId;

	
};
