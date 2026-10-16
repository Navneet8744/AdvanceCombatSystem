// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyController.h"
#include "Enemy/Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

const FName AEnemyController::TargetActorKey = TEXT("TargetActor");
const FName AEnemyController::TargetLocationKey = TEXT("TargetLocation");
const FName AEnemyController::IdealRangeKey = TEXT("AttackRadius");
const FName AEnemyController::DefendRangeKey = TEXT("DefendRadius");
const FName AEnemyController::LastSeenLocationKey = TEXT("LastSeenLocation");
const FName AEnemyController::InvestigateLocationKey = TEXT("InvestigateLocationKey");
const FName AEnemyController::DamageLocationKey = TEXT("DamageLocation");

AEnemyController::AEnemyController()
{

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PercetionComponent");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");

	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1400.f;
	SightConfig->PeripheralVisionAngleDegrees = 65.f;
	SightConfig->SetMaxAge(20.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("HearingConfig");

	HearingConfig->HearingRange = 800.f;
	HearingConfig->SetMaxAge(5.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>("DamageConfig");
	DamageConfig->SetMaxAge(5.f);


	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);

	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	TeamId = FGenericTeamId(2);
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

		if (LastDamageStimulus.IsValid())
		{
			if (LastDamageStimulus.IsExpired())
			{
				Blackboard->ClearValue(TEXT("DamageLocation"));

				LastDamageStimulus = FAIStimulus();
			}
		}


		if (LastHearingStimulus.IsValid())
		{
			if (LastDamageStimulus.IsExpired())
			{
				Blackboard->ClearValue(TEXT("InvestigateLocation"));

				LastDamageStimulus = FAIStimulus();
			}
			
		}
		
}
	
	

	



void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);



	AEnemy* EnemyPawn = Cast<AEnemy>(InPawn);

	if (!EnemyPawn) return;

	if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(InPawn))
	{
		TeamId = TeamAgent->GetGenericTeamId();
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AEnemyController::OnTargetPerceptionForgotten);

	if (EnemyPawn->EnemyData.BehaviorTree)
	{
		RunBehaviorTree(EnemyPawn->EnemyData.BehaviorTree);
	}

	GetBlackboardComponent()->SetValueAsFloat(IdealRangeKey, EnemyPawn->EnemyData.AttackRadius);
	GetBlackboardComponent()->SetValueAsFloat(DefendRangeKey, EnemyPawn->EnemyData.DefendRadius);

}


void AEnemyController::OnUnPossess()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveAll(this);
	AIPerceptionComponent->OnTargetPerceptionForgotten.RemoveAll(this);


}






void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	
	

	if (GetTeamAttitudeTowards(*Actor) != ETeamAttitude::Hostile)
	{
		return;
	}


	

	TSubclassOf<UAISense> Sense = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);

	if (Sense == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			Blackboard->ClearValue(TargetLocationKey);
			Blackboard->ClearValue(LastSeenLocationKey);
			Blackboard->ClearValue(DamageLocationKey);
			Blackboard->ClearValue(InvestigateLocationKey);
			Blackboard->SetValueAsObject(TargetActorKey, Actor);
		
			
			
		}

		else
		{
				Blackboard->ClearValue(TargetActorKey);
				Blackboard->ClearValue(TargetLocationKey);
				Blackboard->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);
			
		}
	}
	else if (Sense == UAISense_Hearing::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (!Blackboard->GetValueAsObject(TargetActorKey))
			{
				Blackboard->ClearValue(LastSeenLocationKey);
				Blackboard->SetValueAsVector(InvestigateLocationKey, Stimulus.StimulusLocation);

			}
		}
		
		LastHearingStimulus = Stimulus;
	}


	else
	{
		if (Stimulus.WasSuccessfullySensed())
		{

			if (!Blackboard->GetValueAsObject(TargetActorKey))
			{
				Blackboard->ClearValue(InvestigateLocationKey);
				Blackboard->ClearValue(LastSeenLocationKey);
				Blackboard->SetValueAsVector(DamageLocationKey,Stimulus.StimulusLocation);
			}

			LastDamageStimulus = Stimulus;

		}

	}

	

}

void AEnemyController::OnTargetPerceptionForgotten(AActor* Actor)
{

	if (!Blackboard) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Perception Forgotten Executed");
		Blackboard->ClearValue(TargetActorKey);
		Blackboard->ClearValue(TargetLocationKey);
		Blackboard->ClearValue(InvestigateLocationKey);
		Blackboard->ClearValue(LastSeenLocationKey);
		Blackboard->ClearValue(DamageLocationKey);
		
	
}




ETeamAttitude::Type AEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);

	if (!OtherTeamAgent)
	{
		return ETeamAttitude::Neutral;
	}

	return (OtherTeamAgent->GetGenericTeamId() == GetGenericTeamId()) ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;

}

FGenericTeamId AEnemyController::GetGenericTeamId() const
{
	return TeamId;
}