
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_BaseEnemy.generated.h"


class AEnemy;
class UCharacterMovementComponent;

UCLASS()
class COMBATDAMAGE_API UAnim_BaseEnemy : public UAnimInstance
{
	GENERATED_BODY()


public:





	UPROPERTY(BlueprintReadOnly)
	AEnemy* Enemy;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling;


	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly)
	float Direction;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* EnemyMovement;

	float CalcDirection();

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeInitializeAnimation() override;
	
};
