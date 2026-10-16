

#include "Components/TargettingComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/I_DamageAble.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Components/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UTargettingComponent::UTargettingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}


void UTargettingComponent::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = Cast<ACharacter>(GetOwner());

	if (MyPlayer)
	{
		OwnerController = MyPlayer->GetController();

		CameraReference = MyPlayer->FindComponentByClass<UCameraComponent>();

		CombatComponent = MyPlayer->FindComponentByClass<UCombatComponent>();
	}
	
	

	if (MyPlayer)
	{
		StartSoftLock();
	}
	  
}





void UTargettingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	IsTargetActorTooFar();
	

	if (IsTargetting && CombatComponent->IsInCombat)
	{
		LockTarget(DeltaTime);
	}

	
}




void UTargettingComponent::LockTarget(float deltaTime)
{
	if (!HardLockActor || !GetOwner() || !OwnerController) return;


	
	if (II_DamageAble::Execute_IsDead(HardLockActor))
	{
		DisableLockOn();
		return;
	}
	

	FRotator CurrentControlRotation =
		OwnerController->GetControlRotation();

	FVector Start = GetOwner()->GetActorLocation();
	FVector  Target = HardLockActor->GetActorLocation() + FVector(0.f, 0.f, 60.f);

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);

	FRotator  LookingRotation = UKismetMathLibrary::RInterpTo(CurrentControlRotation, LookAtRotation, deltaTime, 20.f);
	
	
		
	OwnerController->SetControlRotation(FRotator(
		CurrentControlRotation.Pitch,
		LookingRotation.Yaw,
		0.f
		
	));
	
	
		
}

TArray<AActor*> UTargettingComponent::FindActorsToLockOn()
{
	TArray<AActor*> PotentialTargets;

	if (GetOwner())
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));


		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(MyPlayer);

		FVector Forward = CameraReference->GetForwardVector();

		FVector Up = CameraReference->GetUpVector();

		FVector RotatedForward = Forward.FVector::RotateAngleAxis(RotationAngleAxis, Up);

		FVector Start = GetOwner()->GetActorLocation();

		FVector End = Start + RotatedForward * 1000.f;

			


		TArray<FHitResult> HitResults;

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, SphereRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

		for (const FHitResult& HitResult : HitResults)
		{

			AActor* HitActor = HitResult.GetActor();

			if (!HitActor) continue;

			if (HitActor->Implements<UI_DamageAble>())
			{
				PotentialTargets.Add(HitActor);
			}



		}
	}
	
	return PotentialTargets;
}

TArray<AActor*> UTargettingComponent::FindActorsToSwitchAround(float TraceSize)
{
	
		TArray<AActor*> PotentialTargets;

		if (!GetOwner())
			return PotentialTargets;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

		ObjectTypes.Add(
			UEngineTypes::ConvertToObjectType(ECC_Pawn)
		);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(MyPlayer);

		FVector Start = GetOwner()->GetActorLocation();

		FVector End = Start + FVector(0.f, 0.f, 1.f);

		TArray<FHitResult> HitResults;

		UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			Start,
			End,
			TraceSize,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResults,
			true
		);

		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();

			if (!HitActor)
				continue;

			if (HitActor == HardLockActor)
				continue;

			if (HitActor->Implements<UI_DamageAble>())
			{
				PotentialTargets.AddUnique(HitActor);
			}
		}


		

		return PotentialTargets;
	
}

void UTargettingComponent::ClosestTarget()
{
	if (!GetOwner() || !CameraReference  || !CombatComponent->IsInCombat) return;

	TArray<AActor*> Targets = FindActorsToLockOn();

	float BestScore = -FLT_MAX;

	AActor* BestTarget = nullptr;



	for (AActor* Target : Targets)
	{
		FHitResult HitResult;

		FVector Start = CameraReference->GetComponentLocation();

		FVector End = Target->GetActorLocation();


		FCollisionQueryParams Params;
		Params.AddIgnoredActor(MyPlayer);


		bool BHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, Params);

		if (BHit && HitResult.GetActor() == Target)
		{
			

			
				float TargetScore = GetActorScore(Target);

				if (TargetScore > BestScore)
				{
					BestScore = TargetScore;
					BestTarget = Target;
				}
			
		}
	}

	if (!BestTarget) return;
	HardLockActor = BestTarget;
	IsTargetting = true;
	
	StopSoftLock();
	UpdateActorRotationMode();
	
	

}

void UTargettingComponent::ClosestTargetToleft()
{

	if (!GetOwner() || !CameraReference || !IsTargetting || !CombatComponent->IsInCombat) return ;

	TArray<AActor*> Targets = FindActorsToSwitchAround(1000.f);

	float BestScore = -FLT_MAX;

	AActor* BestTarget = nullptr;

	for (AActor* Target : Targets)
	{
		
		FHitResult HitResult;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(MyPlayer);


		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraReference->GetComponentLocation(), Target->GetActorLocation(), ECollisionChannel::ECC_Pawn,Params);


		if (bHit && HitResult.GetActor() == Target)
		{

				float TargetScore = CheckTargetSwitchActor(HardLockActor, Target, true);
				if (TargetScore > BestScore)
				{
					BestScore = TargetScore;
					BestTarget = Target;
				}
			


		}


	}



	if (!BestTarget) return;
	HardLockActor = BestTarget;
	
	UpdateActorRotationMode();
	
}

void UTargettingComponent::ClosestTargetToRight()
{
	if (!GetOwner() || !CameraReference || !IsTargetting || !CombatComponent->IsInCombat) return ;

	TArray<AActor*> Targets = FindActorsToSwitchAround(1000.f);

	float BestScore = -FLT_MAX;

	AActor* BestTarget = nullptr;

	for (AActor* Target : Targets)
	{

		FHitResult HitResult;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(MyPlayer);


		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraReference->GetComponentLocation(), Target->GetActorLocation(), ECollisionChannel::ECC_Pawn,Params);


		if (bHit && HitResult.GetActor() == Target)
		{


			


				float TargetScore = CheckTargetSwitchActor(HardLockActor, Target, false);
				if (TargetScore > BestScore)
				{
					BestScore = TargetScore;
					BestTarget = Target;
				}
			


		}


	}

	if (!BestTarget) return;
	
		HardLockActor = BestTarget;

		UpdateActorRotationMode();

	
}

float UTargettingComponent::CheckTargetSwitchActor(AActor* CurrentTarget,AActor* Candidate,bool bSwitchLeft)
{
	if (!CurrentTarget || !Candidate || !CameraReference)
		return -9999.f;

	
	FVector ToCandidate =
		(Candidate->GetActorLocation() -
			CurrentTarget->GetActorLocation()).GetSafeNormal();

	// Camera right vector determines left/right
	float Side = FVector::DotProduct(
		CameraReference->GetRightVector(),
		ToCandidate
	);

	// Left
	if (bSwitchLeft)
	{
		if (Side >= 0.f)
			return -9999.f;
	}
	// Right
	else
	{
		if (Side <= 0.f)
			return -9999.f;
	}

	// Distance between current target and candidate
	float Distance = FVector::Dist(
		CurrentTarget->GetActorLocation(),
		Candidate->GetActorLocation()
	);

	const float MaxDistance = 500.f;

	if (Distance > MaxDistance)
		return -9999.f;

	// How strongly the candidate is on the desired side
	float SideScore = FMath::Abs(Side);

	// Prefer closer targets
	float DistanceScore = 1.f - (Distance / MaxDistance);

	// Final score
	return
		(SideScore * 2.f) +
		(DistanceScore * 0.5f);
}

 



float UTargettingComponent::GetActorScore(AActor* Target)
{
	
		if (!Target) return -1.f;

		FVector DirectionToTarget =
			(Target->GetActorLocation() -CameraReference->GetComponentLocation()).GetSafeNormal();

		float DotProduct =
			FVector::DotProduct(
				CameraReference->GetForwardVector(),
				DirectionToTarget
			);

		// Always require target to be in front
		if (DotProduct < 0.3f)
		{
			return -9999.f;
		}

		float Distance =FVector::Dist(CameraReference->GetComponentLocation(),Target->GetActorLocation());

		const float MaxDistance = 500.f;

		if (Distance > MaxDistance)
		{
			return -9999.f;
		}
		
		float CompressedDistance = Distance / MaxDistance;
		float WeightDistance = 0.6f;

		float Score =
			(DotProduct * 2.f) -
			(CompressedDistance * WeightDistance);

		return Score;
	
}



void UTargettingComponent::UpdateActorRotationMode()
{
	if (IsTargetting)
	{
		PrimaryComponentTick.SetTickFunctionEnable(true); 
		SetActorRotationMode(ERotationMode::ERM_OrientToCamera);
		
	}
	else 
	{
		
		SetActorRotationMode(ERotationMode::ERM_OrientToMovement);
		

	}


}

void UTargettingComponent::SetActorRotationMode(ERotationMode RotationMode)
{
	switch (RotationMode)
	{
	case ERotationMode::ERM_OrientToCamera:  
		MyPlayer->bUseControllerRotationYaw = false;
		MyPlayer->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		MyPlayer->GetCharacterMovement()->bOrientRotationToMovement = false;
		break;

	case ERotationMode::ERM_OrientToMovement:
		MyPlayer->bUseControllerRotationYaw = false;
		MyPlayer->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		MyPlayer->GetCharacterMovement()->bOrientRotationToMovement = true;
		break;
	default:
		break;
	}
}

void UTargettingComponent::DisableLockOn()
{
	IsTargetting = false;
	HardLockActor = nullptr;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	

	UpdateActorRotationMode();
	StartSoftLock();


	
}



bool UTargettingComponent::IsTargetActorTooFar()
{
	if (!HardLockActor || !IsTargetting)  return false;

	double DistanceBTWThem = FVector::Dist(HardLockActor->GetActorLocation(), MyPlayer->GetActorLocation());

	if (DistanceBTWThem > FarTargetDistance)
	{
		DisableLockOn();
		return true;
	}

	return false;

}




void UTargettingComponent::StopSoftLock()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SoftLockLoopTimer);
	}
	SoftLockActor = nullptr;
}




void UTargettingComponent::StartSoftLock()
{
	


	if (UWorld* World = GetWorld())
	{
		if (!World->GetTimerManager().IsTimerActive(SoftLockLoopTimer))
		{
			World->GetTimerManager().SetTimer(SoftLockLoopTimer, this, &UTargettingComponent::SoftLockLoop, 0.2f, true);
		}
	}


}



void UTargettingComponent::SoftLockLoop()
{
	if (HardLockActor || IsTargetting)
	{
		SoftLockActor = nullptr;
		return;
	}

	TArray<AActor*> Targets = FindActorsToSwitchAround(SoftLockMaxDistance);

	float BestScore = -FLT_MAX;

	AActor* BestTarget = nullptr;
	
	const FVector CamLoc = CameraReference->GetComponentLocation();
	const FVector CamForward = CameraReference->GetForwardVector();



	for (AActor* Target : Targets)
	{

		if (!IsValid(Target)) continue;

		if (Target->Implements<UI_DamageAble>() && II_DamageAble::Execute_IsDead(Target))
		{
			continue;
		}

		const FVector TargetLoc = Target->GetActorLocation();

		const float Distance = FVector::Dist(CamLoc, TargetLoc);


		if (Distance > SoftLockMaxDistance)
		{
			continue; // Too far
		}


		const FVector DirectionToTarget = (TargetLoc - CamLoc).GetSafeNormal();
		const float DotToCenter = FVector::DotProduct(CamForward, DirectionToTarget);
		if (DotToCenter < SoftLockMinViewDot)
		{
			continue; // Outside the central camera cone
		}

		FHitResult HitResult;


		FVector Start = CameraReference->GetComponentLocation();
		FVector End = Target->GetActorLocation();


		FCollisionQueryParams Params;
		Params.AddIgnoredActor(MyPlayer);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, Params);


		if (!bHit) continue;

		if (HitResult.GetActor() != Target) continue;

		float TargetScore = DotToCenter * 2.0f - (Distance / SoftLockMaxDistance);

		if (TargetScore > BestScore)
		{
			BestScore = TargetScore;
			BestTarget = Target;
		}


	}

	
	SoftLockActor = BestTarget;
	



}





AActor* UTargettingComponent::GetCurrentTarget()
{
	return IsTargetting && HardLockActor ? HardLockActor : SoftLockActor;
}