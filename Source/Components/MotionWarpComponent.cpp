#include "Components/MotionWarpComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interfaces/I_DamageAble.h"
#include "MotionWarpingComponent.h"

UMotionWarpComponent::UMotionWarpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMotionWarpComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		MotionWarpingComponent = Character->FindComponentByClass<UMotionWarpingComponent>();
	}
}



FVector UMotionWarpComponent::CalculateWarpLocation(AActor* Target, const FWarpingSetting& Settings) const
{
	if (!IsValid(Target)) return FVector::ZeroVector;

	FVector Direction = FVector::ZeroVector;

	switch (Settings.WarpPosition)
	{
	case EWarpPosition::EWP_Front: Direction = Target->GetActorForwardVector(); break;
	case EWarpPosition::EWP_Back:  Direction = -Target->GetActorForwardVector(); break;
	case EWarpPosition::EWP_Left:  Direction = -Target->GetActorRightVector(); break;
	case EWarpPosition::EWP_Right: Direction = Target->GetActorRightVector(); break;
	default: break;
	}

	FVector BaseLocation = Target->GetActorLocation();

	if (!Settings.TargetSocketName.IsNone())
	{
		if (USkeletalMeshComponent* TargetMesh = Target->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (TargetMesh->DoesSocketExist(Settings.TargetSocketName))
			{
				BaseLocation = TargetMesh->GetSocketLocation(Settings.TargetSocketName);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("CalculateWarpLocation: Socket %s not found On Target!"), *Settings.TargetSocketName.ToString());
			}
		}

			
	}

	return BaseLocation + (Direction * Settings.WarpDistance);
}





FRotator UMotionWarpComponent::CalculateWarpRotation(AActor* Target, const FVector& ExactTargetLocation) const
{
	if (!IsValid(Character) || !IsValid(Target)) return FRotator::ZeroRotator;

	FVector Direction = ExactTargetLocation - Character->GetActorLocation();
	Direction.Z = 0.f;

	if (Direction.IsNearlyZero()) return Character->GetActorRotation();

	return Direction.Rotation();
}

void UMotionWarpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



bool UMotionWarpComponent::SetupAttackWarp(AActor* Target, const FWarpingSetting& Settings)
{
	

	if (!Settings.bUseWarping)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupAttackWarp: Warping disabled"));
		return false;
	}

	if (Settings.WarpTargetName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("SetupAttackWarp: WarpTargetName is None!"));
		return false;
	}

	if (!IsValid(Character))
	{
		Character = Cast<ACharacter>(GetOwner());
	}

	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("SetupAttackWarp: Character invalid"));
		return false;
	}

	if (!IsValid(MotionWarpingComponent))
	{
		MotionWarpingComponent = Character->FindComponentByClass<UMotionWarpingComponent>();
	}

	if (!IsValid(MotionWarpingComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("SetupAttackWarp: UMotionWarpingComponent missing on Character BP!"));
		return false;
	}

	if (!IsValid(Target))
	{
		UE_LOG(LogTemp, Error, TEXT("SetupAttackWarp: Target invalid"));
		ClearAttackWarp(Settings.WarpTargetName);
		return false;
	}
	
	if (Target->Implements<UI_DamageAble>())
	{
		if (II_DamageAble::Execute_IsDead(Target))
		{
			ClearAttackWarp(Settings.WarpTargetName);
			return false;
		}
	}




	FVector BaseTargetLocation;

	if(IsValid(Target))  BaseTargetLocation = Target->GetActorLocation();



	//if (!Settings.TargetSocketName.IsNone() && Target)
	//{
	//	if (USkeletalMeshComponent* TargetMesh = Target->FindComponentByClass<USkeletalMeshComponent>())
	//	{
	//		if (TargetMesh->DoesSocketExist(Settings.TargetSocketName))
	//		{
	//			BaseTargetLocation = TargetMesh->GetSocketLocation(Settings.TargetSocketName);
	//		}
	//	}
	//}

	
	const float Distance = FVector::Dist2D(Character->GetActorLocation(), BaseTargetLocation);


	if (Distance > Settings.MaxWarpDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupAttackWarp: Target too far. Distance=%f Max=%f"), Distance, Settings.MaxWarpDistance);
		ClearAttackWarp(Settings.WarpTargetName);
		return false;
	}

	const FVector WarpLocation = CalculateWarpLocation(Target, Settings);

	if (Settings.bWarpRotation)
	{

		const FRotator WarpRotation = CalculateWarpRotation(Target,BaseTargetLocation);

		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
			Settings.WarpTargetName,
			WarpLocation,
			WarpRotation
		);
	}
	else
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(
			Settings.WarpTargetName,
			WarpLocation
		);
	}

	return true;
}



void UMotionWarpComponent::ClearAttackWarp(FName WarpTargetName)
{
	if (IsValid(MotionWarpingComponent) && !WarpTargetName.IsNone())
	{
		MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
	}
}