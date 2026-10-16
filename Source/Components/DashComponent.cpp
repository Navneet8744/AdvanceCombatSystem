

#include "Components/DashComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UDashComponent::UDashComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DashTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("DashTimeLineComponent");

}


void UDashComponent::BeginPlay()
{
	Super::BeginPlay();


	if (GetOwner())
	{
		Character = Cast<ACharacter>(GetOwner());
	}



	if (TimelineCurve)
	{
		TimelineTick.BindDynamic(this, &UDashComponent::TimelineUpdate);

		TimelineFinish.BindDynamic(this, &UDashComponent::TimelineFinished);

		DashTimelineComponent->AddInterpFloat(TimelineCurve, TimelineTick);

		DashTimelineComponent->SetTimelineFinishedFunc(TimelineFinish);


	}

	
	
}



void UDashComponent::MoveOverTime(FVector Location)
{
	if (Character)
	{
		DashStartLocation = Character->GetActorLocation();
		DashTargetLocation = Location;

		/*FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(DashStartLocation, Location);
		FRotator GroundRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);

		Character->SetActorRotation(GroundRotation);*/

		if (DashTimelineComponent)
		{
			DashTimelineComponent->SetPlayRate(1.f / DashDuration);
			DashTimelineComponent->PlayFromStart();
		}
	}
}

void UDashComponent::PerformDash(float DashAngle)
{
	if (!Character) return;
	DashData.DashAngle = DashAngle;

	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	Context.AddObjectParam(this);
	Context.AddStructParam(DashData);

	FInstancedStruct ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(DashChooser);

	UAnimMontage* AnimMontage = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UAnimMontage::StaticClass(), false));

	if (!AnimMontage || !Character) return;
	
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	
	FVector DashDirection = Character->GetActorForwardVector().RotateAngleAxis(DashAngle, FVector::UpVector);

	FVector StartLoc = Character->GetActorLocation();
	FVector EndLoc = StartLoc + (DashDirection * DashDistance);

	FVector DashLocation = GetFurthestValidLocation(StartLoc,EndLoc);

	float MontagePlayRate = (DashDuration > 0.0f) ? (AnimMontage->GetPlayLength() / DashDuration) : 1.0f;
	AnimInstance->Montage_Play(AnimMontage, MontagePlayRate);


	MoveOverTime(DashLocation);
}


FVector UDashComponent::GetFurthestValidLocation(FVector StartPoint, FVector EndPoint)
{
	if (!Character || !Character->GetCapsuleComponent()) return StartPoint;

	FVector UnitDirection = (EndPoint - StartPoint).GetSafeNormal();

	float CapsuleRadius = Character->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	float CapsuleDiameter = CapsuleRadius * 2.0f;

	if (CapsuleDiameter <= 0.0f) return StartPoint;

	float Distance = FVector::Dist(StartPoint, EndPoint);
	int32 CapsuleCount = Distance / CapsuleDiameter;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);


	while (CapsuleCount > 0)
	{
		FVector TraceLocation = StartPoint + (CapsuleCount * CapsuleDiameter * UnitDirection);

		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::CapsuleTraceMultiForObjects(
			Character->GetWorld(),
			TraceLocation,
			TraceLocation,
			CapsuleRadius,
			CapsuleHalfHeight,
			ObjectType,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResults,
			true
		);

		if (IsValidDashLocation(TraceLocation, HitResults))
		{
			return TraceLocation;
		}

		
		CapsuleCount--;
	}

	return StartPoint;
}

bool UDashComponent::IsValidDashLocation(const FVector& Location, const TArray<FHitResult>& HitResults)
{
	if (!GetWorld() || !Character) return false;

	// 1. Verify walkable ground exists underneath
	FVector DownTraceTarget = Location - FVector(0.f, 0.f, 500.f);
	FCollisionQueryParams GroundParams;
	GroundParams.AddIgnoredActor(Character);

	FHitResult GroundHit;
	bool bHasGround = GetWorld()->LineTraceSingleByChannel(GroundHit, Location, DownTraceTarget, ECC_Visibility, GroundParams);

	if (!bHasGround)
	{
		return false;
	}

	
	for (const FHitResult& Result : HitResults)
	{
		if (UPrimitiveComponent* HitComponent = Result.GetComponent())
		{
			if (HitComponent->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Block)
			{
				return false;
			}
		}
	}

	return true;
}




void UDashComponent::TimelineUpdate(float Alpha)
{
	FVector LocationToSet = FMath::Lerp(DashStartLocation, DashTargetLocation, Alpha);

	if (Character)
	{
		Character->SetActorLocation(LocationToSet);
	}
}

void UDashComponent::TimelineFinished()
{
}

void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

