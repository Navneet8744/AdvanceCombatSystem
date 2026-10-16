// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interfaces/I_DamageAble.h"
#include "ENUM/ENUM.h"
#include "GenericTeamAgentinterface.h"
#include "MyCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UC_DamageSystem;
class UCombatComponent;
class UTargettingComponent;
class UMotionWarpingComponent;
class UCollisionComponent;
class UDashComponent;
class UMotionWarpComponent;
class AItemBase;
class UParticleSystem;
class UAnimInstance;
class UAIPerceptionStimuliSourceComponent;
class UCameraSetComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);






UCLASS(abstract)
class COMBATDAMAGE_API AMyCharacter : public ACharacter, public II_DamageAble, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:



	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Interaction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;



	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Input")
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TargetLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RightTargetLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LeftTargetLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DamageAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DashAction;



	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAIPerceptionStimuliSourceComponent* StimuliSource;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Components")
	UC_DamageSystem* C_DamageSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTargettingComponent* TargettingComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionWarpingComponent* MotionWarpingComponent;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionWarpComponent* MotionWarpComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UDashComponent* DashComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UCameraSetComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	AItemBase* OverlappingItem;


	
	
	TArray<AItemBase*> Items;

public:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	EItemType ItemType = EItemType::EIT_None;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EActionState ActionState = EActionState::EAS_UnOccupied;


	UPROPERTY(VisibleAnywhere)
	EAttackHoldState AttackHoldState = EAttackHoldState::EHS_None;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	AMyCharacter();	



	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool IsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsTargetting;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool IsJumping;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool DoHeavyAttack;


	

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	AItemBase* CurrentItem;


	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* TeleportBeginParticle;



	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* TeleportEndParticle;
	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;


	FTimerHandle AttackTypeTimer;
	FTimerHandle AttackTypeResetTimer;


	UPROPERTY(EditAnywhere, Category = "Team ID")
	FGenericTeamId TeamID;
	


	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void RunStart();

	void RunStop();


	virtual float GetMaxHealth_Implementation() const override;

	virtual float GetCurrentHealth_Implementation() const override;

	virtual float Heal_Implementation(float HealAmount) override;

	virtual float   TakeDamage_Implementation(FAttackData& AttackData) override;

	virtual void EnterCombat_Implementation() override;

	virtual void ExitCombat_Implementation() override;

	virtual void CollisionTrace_Start_Implementation() override;

	virtual void CollisionTrace_End_Implementation() override;

	virtual void SetActionState_Implementation(EActionState State) override;




 

	UFUNCTION()
	void DamageResponse(FAttackData& AttackData);


	UFUNCTION()
	void BlockResponse(bool CanBeParried);


	UFUNCTION()
	void DeadResponse(FAttackData& AttackData);




	void Interact();
	void UpdateItem();


	void Dash();

	
   
	void LeftTargetLock();
	void TargetLock();
	void RightTargetLock();



	virtual FGenericTeamId GetGenericTeamId() const override;
	

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();




	void DamageTypeChooser();

	void DamageEnemy();

	

	FCombatInfo GetEnemyInfo();


	

	UPROPERTY(VisibleAnywhere)
	int32 DistanceToTarget;

	TArray<AActor*> ActorsDamaged;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UCombatComponent* GetCombatComponent() const { return CombatComponent; }

 	

	FORCEINLINE void SetupOverlappingItem(AItemBase* BaseItem) { OverlappingItem = BaseItem; }
};

