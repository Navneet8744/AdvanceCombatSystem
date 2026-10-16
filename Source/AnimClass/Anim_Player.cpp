


#include "AnimClass/Anim_Player.h"
#include "Player/MyCharacter.h"
#include "Components/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnim_Player::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<AMyCharacter>(TryGetPawnOwner());

	if (Player)
	{
		PlayerMovement = Player->GetCharacterMovement();

	  CombatComponent =	Player->GetCombatComponent();
	}




}

float UAnim_Player::GetDirection()
{
	FVector ForwardVector = Player->GetActorForwardVector();
	FVector RightVector = Player->GetActorRightVector();

	float ForwardDot = FVector::DotProduct(ForwardVector, Player->GetVelocity().GetSafeNormal());	
	float RightDot = FVector::DotProduct(RightVector, Player->GetVelocity().GetSafeNormal());


	float  AngleRad = FMath::Atan2(RightDot, ForwardDot);
	float Degree = FMath::RadiansToDegrees(AngleRad);

	return Degree;
}








void UAnim_Player::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);


	if (PlayerMovement)
	{
		GroundSpeed = PlayerMovement->Velocity.Size();

		Direction = GetDirection();


		IsFalling = PlayerMovement->IsFalling();
	}

}
