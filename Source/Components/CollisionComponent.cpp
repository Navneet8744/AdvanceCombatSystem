


#include "Components/CollisionComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"


UCollisionComponent::UCollisionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	TraceStartPoint = CreateDefaultSubobject<USceneComponent>("TraceStartPoint");
	TraceStartPoint->SetupAttachment(this);

	
	TraceEndPoint = CreateDefaultSubobject<USceneComponent>("TraceEndPoint");
	TraceEndPoint->SetupAttachment(this);
	
}



void UCollisionComponent::ActivateCollision()
{

	if (ItemBase && !OwnerPlayer)
	{
		OwnerPlayer = Cast<ACharacter>(ItemBase->GetOwner());
	}

	if (TraceStartPoint && TraceEndPoint)
	{
		LastStartPos = TraceStartPoint->GetComponentLocation();
		LastEndPos = TraceEndPoint->GetComponentLocation();
		bHasPreviousPositions = true;

	}
	

	IsCollisionEnabled = true;

	
}



void UCollisionComponent::DeactivateCollision()
{
	IsCollisionEnabled = false;
	bHasPreviousPositions = false;
}





void UCollisionComponent::CollisionTrace()
{
	const FVector CurrentStart = TraceStartPoint->GetComponentLocation();
	const FVector CurrentEnd = TraceEndPoint->GetComponentLocation();

	if (!bHasPreviousPositions)
	{
		LastStartPos = CurrentStart;
		LastEndPos = CurrentEnd;
		bHasPreviousPositions = true;
		return;
	}

	constexpr float Radius = 15.f;

	// More substeps the faster it moves, so gaps never exceed the trace radius
	const float MaxMove = FMath::Max(FVector::Dist(LastStartPos, CurrentStart),
		FVector::Dist(LastEndPos, CurrentEnd));
	const int32 Steps = FMath::Clamp(FMath::CeilToInt(MaxMove / Radius), 1, 20);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	if (OwnerPlayer) ActorsToIgnore.Add(OwnerPlayer);

	TArray<FHitResult> CombinedHits;
	TSet<AActor*> HitActors;

	for (int32 i = 1; i <= Steps; ++i)
	{
		const float Alpha = static_cast<float>(i) / Steps;
		const FVector S = FMath::Lerp(LastStartPos, CurrentStart, Alpha);
		const FVector E = FMath::Lerp(LastEndPos, CurrentEnd, Alpha);

		TArray<FHitResult> Hits;
		UKismetSystemLibrary::SphereTraceMultiForObjects(this, S, E, Radius, ObjectType, false,
			ActorsToIgnore, EDrawDebugTrace::None, Hits, true);

		for (const FHitResult& Hit : Hits)
		{
			if (AActor* A = Hit.GetActor())
			{
				bool bAlready = false;
				HitActors.Add(A, &bAlready);
				if (!bAlready) CombinedHits.Add(Hit);
			}
		}
	}

	LastStartPos = CurrentStart;
	LastEndPos = CurrentEnd;

	if (CombinedHits.Num() > 0)
	{
		DamageAllTeamMember(CombinedHits);
	}
}



void UCollisionComponent::AttachTracePoints(USceneComponent* ActorToAttachWith)
{
	if (TraceStartPoint && TraceEndPoint && ActorToAttachWith)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, true);

		
		TraceStartPoint->AttachToComponent(ActorToAttachWith, TransformRules);
		TraceEndPoint->AttachToComponent(ActorToAttachWith, TransformRules);
	}
}




void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemBase = GetOwner();




	
}



void UCollisionComponent::DamageAllTeamMember(const TArray<FHitResult> Hits)
{
	OnHitResponse.Broadcast(Hits);
}




void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsCollisionEnabled)
	{
		CollisionTrace();
	}


}

