

#include "AI/PatrolPoint/PatrolPoints.h"
#include "Components/SplineComponent.h"




APatrolPoints::APatrolPoints()
{
	PrimaryActorTick.bCanEverTick = false;

	PatrolRoute = CreateDefaultSubobject<USplineComponent>("PatrolRoute");
	SetRootComponent(PatrolRoute);

}

void APatrolPoints::IncrementPatrolPoint()
{
	int32 MaxIndex = PatrolRoute->GetNumberOfSplinePoints() - 1;

	PatrolIndex = PatrolIndex + Direction;

	if (PatrolIndex >= MaxIndex)
	{
		PatrolIndex = MaxIndex;
		Direction = -1;
	}

	else if (PatrolIndex <= 0)
	{
		PatrolIndex = 0;
		Direction = 1;
	}
}

FVector APatrolPoints::GetSplinePointAsWorldPosition()
{
	return PatrolRoute->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

void APatrolPoints::BeginPlay()
{
	Super::BeginPlay();
	
}

void APatrolPoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

