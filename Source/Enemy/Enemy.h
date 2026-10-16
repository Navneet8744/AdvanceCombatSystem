#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/I_DamageAble.h"
#include "Interfaces/I_EnemyAI.h"
#include "Enemy.generated.h"














class UBehaviorTree;
class UC_DamageSystem;
class UHitImpactComponent;
class UWidgetComponent;
class UCameraSetComponent;
class UMotionWarpComponent;
class APatrolRute;

USTRUCT(BlueprintType)

struct FEnemyData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float DefendRadius;


	UPROPERTY(EditAnywhere, Category = "AI")
	APatrolPoints* PatrolPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TMap<EMovementState, float> MovementSpeeds;
};



UCLASS()
class AEnemy : public ACharacter, public II_DamageAble , public II_EnemyAI
{
	GENERATED_BODY()

public:
	// Constructor
	AEnemy();

	UPROPERTY(EditAnywhere)
	FEnemyData EnemyData;

	UPROPERTY(VisibleAnywhere, Category = "States")
	EEnemyState  EnemyState = EEnemyState::EES_Passive;

	UPROPERTY(VisibleAnywhere, Category = "States")
	EActionState ActionState = EActionState::EAS_UnOccupied;

	UPROPERTY(VisibleAnywhere, Category = "States")
	EAirDamageState AirDamageState = EAirDamageState::EAAS_OnGround;

protected:
	virtual void BeginPlay() override;

		 
	// ===== COMPONENTS =====
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UC_DamageSystem* C_DamageSystem;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UHitImpactComponent* HitImpactComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraSetComponent* PlayerCameraComponent;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionWarpComponent* MotionWarpComponent;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEnemyWidgetClass> HealthWidgetClass;

public:

	// ===== DAMAGE INTERFACE =====
	//virtual float GetMaxHealth_Implementation() const override;
	virtual float GetCurrentHealth_Implementation() const override;
	virtual float Heal_Implementation(float HealAmount) override;
	virtual float TakeDamage_Implementation(FAttackData& AttackData) override;
	virtual bool IsDead_Implementation() override;
	virtual void  UpdateHealthUI_Implementation(float Percent) override;

	virtual void SetActionState_Implementation(EActionState State) override;


	// ===== EnemyAI INTERFACE ====
	virtual APatrolPoints* GetPatrolPoint_Implementation() const override;
	void SetMovementState_Implementation(EMovementState MovementState) override;
	EEnemyState GetEnemyState_Implementation() const override;
	virtual void SetExecution_Implementation(bool CanExecute) override;
	virtual bool  CanBeExecute_Implementation() override;
	virtual void SetAirDamageState_Implementation(EAirDamageState DamageState) override;
	virtual EAirDamageState GetAirDamageState_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void EnemyDie();
	
	void SetEnemyState(EEnemyState State);

protected:

	// ===== RESPONSES =====
	UFUNCTION()
	void DamageResponse(FAttackData& AttackData);

	UFUNCTION()
	void BlockResponse(bool CanBeParried);

	UFUNCTION()
	void DeadResponse(FAttackData& AttackData);

	UFUNCTION()
	void SetEnemyhealth(float Percent);


	

public:

	FORCEINLINE	EEnemyState GetEnemyState()  const { return EnemyState; }

	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	
};