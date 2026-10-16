

#include "Components/CombatComponent.h"
#include "Player/MyCharacter.h"
#include "Items/ItemBase.h"
#include "Items/Weapons/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"


UCombatComponent::UCombatComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	

	
}



void UCombatComponent::DoAttack(FCombatInfo CombatInfo)
{
	
	if (!IsValid(MyCharacter) || MyCharacter->ActionState == EActionState::EAS_Engaged) return;

	if (!IsInCombat) return;

	
	MyCharacter->AttackHoldState = EAttackHoldState::EHS_None;

	if(!CurrentWeapon) CurrentWeapon = Cast<AWeaponBase>(MyCharacter->CurrentItem);

	if (CombatInfo.CanDoExecution)
	{
		CurrentWeapon->ExecutionAttack(CombatInfo);
		return;
	}

	else if (CombatInfo.AirDamageState == EAirDamageState::EAAS_InAir)
	{
		CurrentWeapon->AirAttack(CombatInfo);
		return;
	}
	else if (CombatInfo.AirDamageState == EAirDamageState::EAAS_OnGround)
	{
		if (CurrentWeapon) CurrentWeapon->NormalAttack(CombatInfo);
	}
	
}

void UCombatComponent::HeavyAttackImplement(FCombatInfo CombatInfo)
{

	if (!MyCharacter) return;
	if (MyCharacter->ActionState == EActionState::EAS_Engaged) return;
 
	
	if (MyCharacter->AttackHoldState != EAttackHoldState::EHS_WaitingForHeavy) return;
	if (!IsInCombat) return;
		
	MyCharacter->AttackHoldState = EAttackHoldState::EHS_HeavyPose;

	if (IsValid(MyCharacter))
	{

		if (!CurrentWeapon) CurrentWeapon = Cast<AWeaponBase>(MyCharacter->CurrentItem);

		if (CurrentWeapon) CurrentWeapon->HeavyAttackImplement(CombatInfo);

		GetWorld()->GetTimerManager().ClearTimer(HeavyAttackHandler);
		GetWorld()->GetTimerManager().SetTimer(HeavyAttackHandler, this, &UCombatComponent::HeavyAttackExecute, 8.f,false);

	}
}

void UCombatComponent::CancelHeavyAttack()
{

	if (MyCharacter && CurrentWeapon)
	{
		
		MyCharacter->AttackHoldState = EAttackHoldState::EHS_None;
		MyCharacter->ActionState = EActionState::EAS_UnOccupied;
		GetWorld()->GetTimerManager().ClearTimer(HeavyAttackHandler);

		CurrentWeapon->CancelHeavyAttack();

		

	}

}






void UCombatComponent::HeavyAttackExecute()
{
	if (!MyCharacter) return;

	if (MyCharacter->AttackHoldState == EAttackHoldState::EHS_None)
	{
		GetWorld()->GetTimerManager().ClearTimer(HeavyAttackHandler);
		return;
	}
	
	if (CurrentWeapon)
	{
		MyCharacter->AttackHoldState = EAttackHoldState::EHS_None;

		CurrentWeapon->HeavyAttackExecute();
	}
}

void UCombatComponent::ExecutionAttack(FCombatInfo CombatInfo)
{
	if (MyCharacter && CurrentWeapon && CombatInfo.AttackTarget)
	{
		CurrentWeapon->ExecutionAttack(CombatInfo);
	}
}


void UCombatComponent::SaveAttack()
{
	ComboCount++;
}



void UCombatComponent::ResetAttack()
{
	ComboCount = 0;
}

void UCombatComponent::StartCollisionTrace()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->CollisionTraceStart();
	}
}

void UCombatComponent::EndCollisionTrace()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->CollisionTraceEnd();
	}
}



void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();


	MyCharacter = Cast<AMyCharacter>(GetOwner());

	
	
}



void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

