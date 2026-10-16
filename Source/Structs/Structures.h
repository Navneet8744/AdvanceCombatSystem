#pragma once


#include "CoreMinimal.h"
#include "ENUM/ENUM.h"
#include "Structures.generated.h"


USTRUCT(BlueprintType)

struct FDamageInfo
{
	GENERATED_BODY()


	AActor* AttackingActor;

	UPROPERTY(BlueprintReadOnly)
	float DamageAmount;

	UPROPERTY(BlueprintReadOnly)
	float ExecutedID;


	UPROPERTY(BlueprintReadOnly)
	EWeaponType WeaponType;



	UPROPERTY(BlueprintReadOnly)
	EAttackType AttackType;

	UPROPERTY(BlueprintReadOnly)
	EDamageResponse DamageResponse;

	UPROPERTY(BlueprintReadOnly)
	FVector HitPoint;

	
	UPROPERTY(BlueprintReadOnly)
	bool ShouldDamageInvincible;   


	UPROPERTY(BlueprintReadOnly)
	bool CanBeBlocked;


	UPROPERTY(BlueprintReadOnly)
	bool CanBeParried;


	UPROPERTY(BlueprintReadOnly)
	bool ShouldForceInterrupt;

	UPROPERTY(BlueprintReadOnly)
	bool IsExecutionAttack;

	UPROPERTY(BlueprintReadOnly)
	bool IsAirAttack;

};


USTRUCT(BlueprintType)
struct FCombatInfo  : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* AttackTarget;

	UPROPERTY(BlueprintReadOnly)
	float TargetHealth;

	UPROPERTY(BlueprintReadOnly)
	int32 ComboCount;

	UPROPERTY(BlueprintReadOnly)
	bool CanDoExecution;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly)
	bool IsTargetLocked;

	UPROPERTY(BlueprintReadOnly)
	bool IsSprinting;

	UPROPERTY(BlueprintReadOnly)
	bool IsBlocking;

	UPROPERTY(BlueprintReadOnly)
	EAirDamageState AirDamageState;

	UPROPERTY(BlueprintReadOnly)
	EAttackRange AttackRange;

	UPROPERTY(BlueprintReadOnly)
	EAttackDirection AttackDirection;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState;


	UPROPERTY(BlueprintReadOnly) 
	int32  DistanceToTarget = 0;




};




USTRUCT(BlueprintType)
struct FWarpingSetting : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) bool bUseWarping = true;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseWarping")) float WarpDistance = 100.f;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseWarping")) float MaxWarpDistance = 300.f;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseWarping")) EWarpPosition WarpPosition = EWarpPosition::EWP_Front;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseWarping")) 	bool bWarpRotation = true;


	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseWarping")) FName WarpTargetName;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseWarping")) FName TargetSocketName;
};



USTRUCT(BlueprintType)
struct FExecutionData : public FTableRowBase
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 HeavyAttackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  bool IsExecutionAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "IsExecutionAttack"))  int32  ExecutionID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "IsExecutionAttack"))  FRotator ExecutionRotation;

};

USTRUCT(BlueprintType)
struct FLaunchData : public FTableRowBase
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool IsAirAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bAirLaunchAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bAirLaunchAttack")) float Target_VerticalLaunchStrength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bAirLaunchAttack")) float Target_HorizontalLaunchStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bAirLaunchAttack")) float Attacker_VerticalLaunchStrength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bAirLaunchAttack")) float Attacker_HorizontalLaunchStrength;




};


USTRUCT(BlueprintType)

struct FAttackData  : public FTableRowBase
{
	GENERATED_BODY()

	
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)   TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(BlueprintReadOnly) AActor* AttackingActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)   float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  FWarpingSetting WarpingSetting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FExecutionData ExecutionSetting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)   EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  EDamageResponse DamageResponse;

	UPROPERTY(BlueprintReadOnly) EAttackDirection DamageDirection;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FLaunchData LaunchData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  bool ShouldDamageInvincible;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  bool CanBeBlocked;

	UPROPERTY(BlueprintReadOnly)  	FVector HitPoint;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)  bool CanBeParried;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  bool ShouldForceInterrupt;


	

 };





