
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPoints.generated.h"

class USplineComponent;
UCLASS()
class COMBATDAMAGE_API APatrolPoints : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolPoints();

	void IncrementPatrolPoint();

	FVector GetSplinePointAsWorldPosition();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USplineComponent* PatrolRoute;


	int32 Direction = 1;

	int32 PatrolIndex = 0;
	

public:	
	virtual void Tick(float DeltaTime) override;

};
