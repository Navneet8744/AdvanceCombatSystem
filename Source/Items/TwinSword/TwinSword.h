#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WeaponBase.h"
#include "ChooserFunctionLibrary.h"
#include "TwinSword.generated.h"


class UStaticMeshComponent;
class UCollisionComponent;
class UHitImpactComponent;

USTRUCT(BlueprintType)

struct FTwinSwordData
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = "Sockets")
	FName EquipSocket_1;

	UPROPERTY(EditAnywhere, Category = "Sockets")
	FName EquipSocket_2;

	UPROPERTY(EditAnywhere, Category = "Sockets")
	FName UnEquipSocket_1;

	UPROPERTY(EditAnywhere, Category = "Sockets")
	FName UnEquipSocket_2;

	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> NormalAttackChooser;

	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> AirAttackChooser;

	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> HeavyAttackImplementChooser;

	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> HeavyAttackExecuteChooser;

	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> ExecutionChooser;


};


UCLASS()
class COMBATDAMAGE_API ATwinSword : public AWeaponBase
{
	GENERATED_BODY()


public:



	ATwinSword();

	
	UPROPERTY(EditAnywhere, Category = "TwinSwordData")
	FTwinSwordData TwinSwordData;
	

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Sword_1;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Sword_2;


	UPROPERTY(EditAnywhere, Category = "Components")
	UCollisionComponent* CollisionComponent_1;

	UPROPERTY(EditAnywhere, Category = "Components")
	UCollisionComponent* CollisionComponent_2;


	virtual void OnHit(TArray<FHitResult> HitResults) override;



	//virtual void UseItem(FCombatInfo CombatInfo) override;

	

	virtual void NormalAttack(FCombatInfo CombatInfo) override;
	virtual void HeavyAttackImplement(FCombatInfo CombatInfo) override;
	virtual void HeavyAttackExecute() override;
	virtual void ExecutionAttack(FCombatInfo CombatInfo) override;
	virtual void AirAttack(FCombatInfo CombatInfo) override;
	virtual void AirAttackEnded(const FAttackData& AttackData, AActor* TargetActor, AActor* Attacker) override;
	virtual void AttachToBody(USkeletalMeshComponent* Mesh) override;

	virtual void AttachToHand(USkeletalMeshComponent* Mesh) override;


	virtual void CollisionTraceStart() override;
	virtual void CollisionTraceEnd() override;


	FAttackData DefaultAttackSetup(AActor* owner, FCombatInfo CombatInfo, const TObjectPtr<UChooserTable> AttackType);
	

protected:

	virtual void BeginPlay() override;

private:

	
};
