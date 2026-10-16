

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENUM/ENUM.h"
#include "TargettingComponent.generated.h"

class UCameraComponent;
class AMyCharacter;
class UCombatComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATDAMAGE_API UTargettingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTargettingComponent();

	UPROPERTY(EditAnywhere, Category = "SphereRadius") float  SphereRadius ;
	UPROPERTY(EditAnywhere, Category = "RotationAngleAxis") float RotationAngleAxis;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting")
	AActor* HardLockActor = nullptr;

	// The target passively detected in front of the player (Soft Lock)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting")
	AActor* SoftLockActor = nullptr;


	UPROPERTY(EditAnywhere, Category = "Soft Lock|Range")
	float SoftLockMaxDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Soft Lock|Range", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SoftLockMinViewDot = 0.8f;

	FTimerHandle SoftLockLoopTimer;



	


	TObjectPtr<ACharacter> MyPlayer;

	AController* OwnerController;

	bool IsTargetting;

	UCameraComponent* CameraReference;

	UCombatComponent* CombatComponent;



	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LockTarget(float deltaTime);


	TArray<AActor*> FindActorsToLockOn();

	TArray<AActor*> FindActorsToSwitchAround(float TraceSize);

	void ClosestTarget();
	void  ClosestTargetToleft();
	void ClosestTargetToRight();

	float CheckTargetSwitchActor(AActor* CurrentTarget , AActor* Candidate, bool bSwitchLeft);

	void UpdateActorRotationMode();

	float GetActorScore(AActor* Target);

	void SetActorRotationMode(ERotationMode RotationMode);

	void DisableLockOn();


	void SoftLockLoop();
	void StartSoftLock();
	void StopSoftLock();

	bool IsTargetActorTooFar();


	AActor* GetCurrentTarget();

	UPROPERTY(EditAnywhere, Category = "FarTargetDistance")
	float FarTargetDistance;


protected:
	virtual void BeginPlay() override;




public:

	FORCEINLINE bool GetIsTargetting() const { return IsTargetting;  }
	

		
};
