
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/Structures.h"
#include "MotionWarpComponent.generated.h"

class ACharacter;
class UMotionWarpingComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATDAMAGE_API UMotionWarpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMotionWarpComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	bool SetupAttackWarp(AActor* Target,const FWarpingSetting& Settings);

	void ClearAttackWarp(FName WarpTargetName);



protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	FVector CalculateWarpLocation(AActor* Target, const FWarpingSetting& Settings) const;

	FRotator CalculateWarpRotation(AActor* Target, const FVector& ExactTargetLocation) const;


		
};
