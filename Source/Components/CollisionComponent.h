

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CollisionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, TArray<FHitResult>, HitResults);

class UBoxComponent;
class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATDAMAGE_API UCollisionComponent : public USceneComponent
{
	GENERATED_BODY()



public:	
	UCollisionComponent();


	UPROPERTY(BlueprintAssignable)
	FOnHit OnHitResponse;


	UPROPERTY(EditAnywhere)
	USceneComponent* TraceStartPoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* TraceEndPoint;

	bool IsCollisionEnabled;


	TObjectPtr<ACharacter> OwnerPlayer;

	TObjectPtr<AActor> ItemBase;


	void ActivateCollision();
	void DeactivateCollision();


	
	void CollisionTrace();


	void AttachTracePoints(USceneComponent* ActorToAttachWith);

protected:



	FVector LastStartPos;
	FVector LastEndPos;
	bool bHasPreviousPositions = false;


	virtual void BeginPlay() override;

	void DamageAllTeamMember(const TArray<FHitResult> Hits);


public:	


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
