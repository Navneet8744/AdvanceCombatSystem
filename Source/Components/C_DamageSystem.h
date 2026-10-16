

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENUM/ENUM.h"
#include "Structs/Structures.h"
#include "C_DamageSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamageResponse,FAttackData&, AttackData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeadResponse,FAttackData&,AttackData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlockResponse, bool, CanBeParried);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetHealthUI, float, DamageAmount);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATDAMAGE_API UC_DamageSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UC_DamageSystem();


	UPROPERTY(BlueprintAssignable)
	FDamageResponse OnDamageResponse;
	
	UPROPERTY(BlueprintAssignable)
	FDeadResponse OnDeadResponse;

	UPROPERTY(BlueprintAssignable)
	FBlockResponse OnBlockResponse;

	UPROPERTY(BlueprintAssignable)
	FSetHealthUI SetHealthUI;

	UPROPERTY(EditAnywhere, Category = "DamageSystem")
	float Health = 100.f;
	UPROPERTY(EditAnywhere, Category = "DamageSystem")
	float Maxhealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "DamageSystem")
	bool IsInvincible;

	UPROPERTY(EditAnywhere, Category = "DamageSystem")
	bool IsDead;

	UPROPERTY(EditAnywhere, Category = "DamageSystem")
	bool IsInterruptible = true;

	UPROPERTY(EditAnywhere, Category = "DamageSystem")
	bool IsBlocking;

	float Heal(float HealAmount);

	bool TakeDamage(FAttackData& AttackData);

	void ReportDamage(FAttackData& AttackData);

	EAttackDirection GetEnemyDirection(AActor* Enemy, AActor* Attacker);

protected:
	
	virtual void BeginPlay() override;

	
 
public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
