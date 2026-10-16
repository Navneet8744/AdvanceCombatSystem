

#include "AnimClass/Anim_BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy.h"









void UAnim_BaseEnemy::NativeInitializeAnimation()
{
   Super::NativeInitializeAnimation();

	Enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (Enemy)
	{
		EnemyMovement = Enemy->GetCharacterMovement();

		
	}
}

float UAnim_BaseEnemy::CalcDirection()
{
	
		if (!Enemy) return 0.f;

		FVector EnemyForward = Enemy->GetActorForwardVector();
		FVector  EnemyRight = Enemy->GetActorRightVector();

		float ForwardDot = FVector::DotProduct(EnemyForward, EnemyMovement->Velocity.GetSafeNormal());
		float RightDot = FVector::DotProduct(EnemyRight, EnemyMovement->Velocity.GetSafeNormal());

		float AngleRad = FMath::Atan2(RightDot, ForwardDot);
		float Degree = FMath::RadiansToDegrees(AngleRad);

		return Degree;

	
}

void UAnim_BaseEnemy::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);


	if (EnemyMovement)
	{
		GroundSpeed = EnemyMovement->Velocity.Size();

		IsFalling = EnemyMovement->IsFalling();

		Direction = CalcDirection();
	}
}

