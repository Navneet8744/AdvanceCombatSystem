


#include "Components/C_DamageSystem.h"
#include "Structs/Structures.h"
#include "Perception/AISense_Damage.h"


UC_DamageSystem::UC_DamageSystem()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}



void UC_DamageSystem::BeginPlay()
{
	Super::BeginPlay();

	
	
}

float UC_DamageSystem::Heal(float HealAmount)
{
	if (IsDead)  return 0.f;
	return  Health = FMath::Clamp(Health + HealAmount, 0.f, Maxhealth);
}



bool UC_DamageSystem::TakeDamage(FAttackData& AttackData)
{
	if (!IsDead && (!IsInvincible || AttackData.ShouldDamageInvincible))
	{
		if (AttackData.CanBeBlocked && IsBlocking)
		{
			OnBlockResponse.Broadcast(AttackData.CanBeParried);
			return false;

		}
		else
		{
			Health = FMath::Clamp(Health - AttackData.Damage, 0.f, Maxhealth);
			ReportDamage(AttackData);
			SetHealthUI.Broadcast(Health/Maxhealth);
			if (Health <= 0)
			{
				IsDead = true;
				OnDeadResponse.Broadcast(AttackData);
				return true;
			}
			else
			{
				if (IsInterruptible || AttackData.ShouldForceInterrupt)
				{
					UE_LOG(LogTemp, Warning, TEXT("DamageResponse : %s"), *UEnum::GetValueAsString(AttackData.DamageResponse));
					OnDamageResponse.Broadcast(AttackData);
					return true;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return false;
	
}

void UC_DamageSystem::ReportDamage(FAttackData& AttackData)
{
	FVector EventLocation = AttackData.AttackingActor->GetActorLocation();
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), AttackData.AttackingActor, 0.f, EventLocation, AttackData.HitPoint);
}

EAttackDirection UC_DamageSystem::GetEnemyDirection(AActor* Enemy, AActor* Attacker)
{
	if (!Enemy || !Attacker) return EAttackDirection::EDD_FromFront;

	FVector DirectionToAttacker = Attacker->GetActorLocation() - Enemy->GetActorLocation();
	DirectionToAttacker.Z = 0.f;
	DirectionToAttacker.Normalize();

	FVector EnemyForward = Enemy->GetActorForwardVector();

	FVector EnemyRight = Enemy->GetActorRightVector();

	float ForwardDot = FVector::DotProduct(EnemyForward, DirectionToAttacker);

	float RightDot = FVector::DotProduct(EnemyRight, DirectionToAttacker);


	if (ForwardDot > 0.5f)
	{
		return EAttackDirection::EDD_FromFront;
	}

	if (ForwardDot < -0.5f)
	{
		return EAttackDirection::EDD_FromBack;
	}


	if (RightDot > 0.f)
	{
		return EAttackDirection::EDD_FromRight;
	}

	return EAttackDirection::EDD_FromLeft;

}



void UC_DamageSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

