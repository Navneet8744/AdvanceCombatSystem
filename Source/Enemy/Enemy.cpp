#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/C_DamageSystem.h"
#include "Enemy/EnemyWidgetClass.h"
#include "Components/HitImpactComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/MotionWarpComponent.h"
#include "Components/CameraSetComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/PatrolPoint/PatrolPoints.h"

// Constructor
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Movement (keep this for AI)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->bOrientRotationToMovement = false;
		MovementComp->bUseControllerDesiredRotation = true;
		MovementComp->RotationRate = FRotator(0.f, 500.f, 0.f);
	}

	


	GetCharacterMovement()->MaxWalkSpeed = 250.f;

	EnemyData.MovementSpeeds.Add(EMovementState::EMS_Idle, 0.f);
	EnemyData.MovementSpeeds.Add(EMovementState::EMS_Walking, 250.f);
	EnemyData.MovementSpeeds.Add(EMovementState::EMS_Jogging, 500.f);
	EnemyData.MovementSpeeds.Add(EMovementState::EMS_Sprinting, 700.f);

	// Damage System
	C_DamageSystem = CreateDefaultSubobject<UC_DamageSystem>(TEXT("C_DamageSystem"));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthWidgetComponent->SetWidgetClass(HealthWidgetClass);

	HealthWidgetComponent->SetupAttachment(GetMesh());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawSize(FVector2D(150.f, 20.f));
	HealthWidgetComponent->SetVisibility(false);

	

	HitImpactComponent = CreateDefaultSubobject<UHitImpactComponent>("HitImpactComponent");

	PlayerCameraComponent = CreateDefaultSubobject<UCameraSetComponent>("PlayerCameraComponent");

	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpComponent>("MotionWarpComponent");
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (C_DamageSystem)
	{
		C_DamageSystem->OnDamageResponse.AddUniqueDynamic(this, &AEnemy::DamageResponse);
		C_DamageSystem->OnBlockResponse.AddUniqueDynamic(this, &AEnemy::BlockResponse);
		C_DamageSystem->OnDeadResponse.AddUniqueDynamic(this, &AEnemy::DeadResponse);
		C_DamageSystem->SetHealthUI.AddUniqueDynamic(this, &AEnemy::SetEnemyhealth);

	}

	
	
	
		
	
}



// ===== DAMAGE INTERFACE =====

//float AEnemy::GetMaxHealth_Implementation() const
//{
//	return C_DamageSystem ? C_DamageSystem->Maxhealth : 0.f;
//}



float AEnemy::GetCurrentHealth_Implementation() const
{
	return C_DamageSystem ? C_DamageSystem->Health : 0.f;
}


float AEnemy::Heal_Implementation(float HealAmount)
{
	return C_DamageSystem ? C_DamageSystem->Heal(HealAmount) : 0.f;
}


float AEnemy::TakeDamage_Implementation(FAttackData& AttackData)
{
	if (!C_DamageSystem) return false;

	return C_DamageSystem->TakeDamage(AttackData);
}

bool AEnemy::IsDead_Implementation()
{
	if (C_DamageSystem)
	{
		return C_DamageSystem->IsDead;
	}
	return false;
}


void AEnemy::UpdateHealthUI_Implementation(float Percent)
{
	if (!HealthWidgetComponent) return;
	{
		auto Widget = Cast<UEnemyWidgetClass>(HealthWidgetComponent->GetUserWidgetObject());

		if (Widget)
		{
			Widget->SetHealthPercent(Percent);
		}
	}
}

void AEnemy::SetExecution_Implementation(bool CanExecute)
{
	if(HitImpactComponent) HitImpactComponent->bCanExecute = CanExecute;
}

bool AEnemy::CanBeExecute_Implementation()
{
	return HitImpactComponent ? HitImpactComponent->bCanExecute : false;
}

void AEnemy::SetAirDamageState_Implementation(EAirDamageState DamageState)
{
	AirDamageState = DamageState;
}

EAirDamageState AEnemy::GetAirDamageState_Implementation()
{
	return AirDamageState;
}





void AEnemy::SetActionState_Implementation(EActionState State)
{

	

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum("ActionState", static_cast<uint8>(State));
	}
	ActionState = State;

}








// ===== RESPONSES =====

void AEnemy::DamageResponse(FAttackData& AttackData)
{

	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetVisibility(true);
	}

	if (HitImpactComponent)
	{
		HitImpactComponent->DoHitReaction(AttackData, false);
		
	
	}



}



void AEnemy::BlockResponse(bool CanBeParried)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Enemy Blocked"));
}




void AEnemy::DeadResponse(FAttackData& AttackData)
{
	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetVisibility(false);
	}
	if (AttackData.ExecutionSetting.IsExecutionAttack) return;
	
	if (!HitImpactComponent) return;

	HitImpactComponent->DoHitReaction(AttackData, true);




	
}

void AEnemy::SetEnemyhealth(float Percent)
{
	UpdateHealthUI_Implementation(Percent);
}







APatrolPoints* AEnemy::GetPatrolPoint_Implementation() const
{
	return EnemyData.PatrolPoint ?  EnemyData.PatrolPoint : nullptr;
}


void AEnemy::SetMovementState_Implementation(EMovementState MovementState)
{

	if (float* Speed = EnemyData.MovementSpeeds.Find(MovementState))
	{
		GetCharacterMovement()->MaxWalkSpeed = *Speed;
	}

}

EEnemyState AEnemy::GetEnemyState_Implementation() const
{
	return GetEnemyState();
}

void AEnemy::EnemyDie()
{
	
	if (!C_DamageSystem->IsDead) return;

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetVisibility(false);
	}
}

void AEnemy::SetEnemyState(EEnemyState State)
{
	EnemyState = State;
}



