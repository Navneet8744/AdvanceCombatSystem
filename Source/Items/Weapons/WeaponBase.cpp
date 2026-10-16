// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponBase.h"
#include "Interfaces/I_DamageAble.h"
#include "Components/CombatComponent.h"
#include "Components/CapsuleComponent.h"

#include "Gameframework/Character.h"


void AWeaponBase::Equip(USkeletalMeshComponent* Mesh, AActor* owner, APawn* instigator)
{
	Super::Equip(Mesh, owner, instigator);

	


	CombatComponent = Character->FindComponentByClass<UCombatComponent>();


	if (!CombatComponent || !owner) return;




	FChooserEvaluationContext  EquipContext = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	EquipContext.AddObjectParam(Character);

	EquipContext.AddObjectParam(CombatComponent);

	FInstancedStruct ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(ItemData.ItemChooser);

	UAnimMontage* EquipMontage = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(EquipContext, ChooserStruct, UAnimMontage::StaticClass(), false));

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	if (AnimInstance && EquipMontage) AnimInstance->Montage_Play(EquipMontage);


}


void AWeaponBase::UnEquip(USkeletalMeshComponent* Mesh)
{
	Super::UnEquip(Mesh);

	if (!CombatComponent || !GetOwner()) return;




	FChooserEvaluationContext  UnEquipContext = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	UnEquipContext.AddObjectParam(GetOwner());
	UnEquipContext.AddObjectParam(CombatComponent);

	FInstancedStruct ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(ItemData.ItemChooser);

	UAnimMontage* UnEquipMontage = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(UnEquipContext, ChooserStruct, UAnimMontage::StaticClass(), false));

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	if (AnimInstance && UnEquipMontage)  AnimInstance->Montage_Play(UnEquipMontage);


}

void AWeaponBase::AttachToBody(USkeletalMeshComponent* Mesh)
{
	if (CombatComponent) CombatComponent->IsInCombat = false;
}

void AWeaponBase::AttachToHand(USkeletalMeshComponent* Mesh)
{

	if (CombatComponent) CombatComponent->IsInCombat = true;

}

void AWeaponBase::CollisionTraceStart()
{
	ActorsDamageTooFar.Empty();
}

void AWeaponBase::CollisionTraceEnd()
{
}


void AWeaponBase::NormalAttack(FCombatInfo CombatInfo)
{

}
void AWeaponBase::HeavyAttackImplement(FCombatInfo CombatInfo)
{

}


void AWeaponBase::CancelHeavyAttack()
{
	if (!Character) return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	
	
	AnimInstance->StopAllMontages(0.15f);
	
}

void AWeaponBase::AirAttack(FCombatInfo CombatInfo)
{



}

void AWeaponBase::AirAttackEnded(const FAttackData& AttackData, AActor* TargetActor, AActor* Attacker)
{
}









void AWeaponBase::HeavyAttackExecute()
{

}

void AWeaponBase::ExecutionAttack(FCombatInfo CombatInfo)
{

}




void AWeaponBase::OnHit(TArray<FHitResult> HitResults)
{

	for (FHitResult HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();

		if (!HitActor->Implements<UI_DamageAble>()) continue;
		if (ActorsDamageTooFar.Contains(HitActor)) continue;

		

		AttackInfo.AttackingActor = GetOwner();
		AttackInfo.HitPoint = HitResult.ImpactPoint;
		II_DamageAble::Execute_TakeDamage(HitActor, AttackInfo);

		ActorsDamageTooFar.AddUnique(HitActor);

	}


}




void AWeaponBase::ExecutionEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}