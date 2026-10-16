// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "Structs/Structures.h"
#include "WeaponBase.generated.h"

class UCombatComponent;
class UHitImpactComponent;
class UMotionWarpComponent;

UCLASS()
class COMBATDAMAGE_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()


public:







	FAttackData AttackInfo;

	

	TArray<AActor*> ActorsDamageTooFar;







	UFUNCTION()
	virtual void OnHit(TArray<FHitResult> HitResults);






	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent;

	


	virtual void NormalAttack(FCombatInfo CombatInfo);
	virtual void HeavyAttackImplement(FCombatInfo CombatInfo);
	virtual void CancelHeavyAttack();
	virtual void AirAttack(FCombatInfo CombatInfo);
	virtual void AirAttackEnded(const FAttackData& AttackData, AActor* TargetActor, AActor* Attacker);
	virtual void HeavyAttackExecute();
	virtual void ExecutionAttack(FCombatInfo ComnbatInfo);



	virtual void Equip(USkeletalMeshComponent* Mesh, AActor* owner, APawn* instigator) override;
	virtual void UnEquip(USkeletalMeshComponent* Mesh) override;


	virtual void AttachToBody(USkeletalMeshComponent* Mesh) override;

	virtual void AttachToHand(USkeletalMeshComponent* Mesh) override;
	

	void ExecutionEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void CollisionTraceStart() override;
	virtual void CollisionTraceEnd() override;
};
