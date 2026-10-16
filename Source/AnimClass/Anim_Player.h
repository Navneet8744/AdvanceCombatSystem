// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_Player.generated.h"

class AMyCharacter;
class UCharacterMovementComponent;
class UCombatComponent;

UCLASS()
class COMBATDAMAGE_API UAnim_Player : public UAnimInstance
{
	GENERATED_BODY()

public:





	UPROPERTY(BlueprintReadOnly)
	bool IsFalling;


	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly)
	float Direction;


	UPROPERTY(BlueprintReadOnly)
	AMyCharacter* Player;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* PlayerMovement;

	UPROPERTY(BlueprintReadOnly)
	UCombatComponent* CombatComponent;



	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeInitializeAnimation() override; 

	float GetDirection();
	
};
