
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/Structures.h"
#include "ChooserFunctionLibrary.h"
#include "CameraSetComponent.generated.h"


class APlayerCameraManager;
class UHitImpactComponent;
class UCameraShakeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATDAMAGE_API UCameraSetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCameraSetComponent();

	UPROPERTY(EditAnywhere,Category = "Chooser")
	TObjectPtr<UChooserTable> CameraShakeChooser;



	UPROPERTY()
	TObjectPtr<UHitImpactComponent> HitImpactComponent;


	APlayerCameraManager* CameraManager;


	void DoCameraShake(FAttackData& AttackData);


	void PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass);
	void PauseCameraShake();

	UCameraShakeBase* RawShakeClass;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
