// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "ChooserFunctionLibrary.h"
#include "DashComponent.generated.h"


USTRUCT(BlueprintType)
struct  FDashData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float DashAngle;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATDAMAGE_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDashComponent();

	UFUNCTION()
	void TimelineUpdate(float Alpha);


	UFUNCTION()
	void TimelineFinished();


	void PerformDash(float DashAngle);

	FVector GetFurthestValidLocation(FVector StartPoint, FVector EndPoint);

	bool IsValidDashLocation(const FVector& Location, const TArray<FHitResult>& HitResults);

protected:

	UPROPERTY(BlueprintReadOnly)
	FDashData DashData;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDuration;


	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDistance;

	UPROPERTY(EditAnywhere, Category = "Dash")
	TObjectPtr<UChooserTable> DashChooser;

	FVector DashStartLocation;

	FVector DashTargetLocation;

	virtual void BeginPlay() override;

	void MoveOverTime(FVector Location);





	TObjectPtr<ACharacter> Character;


	UPROPERTY()
	UTimelineComponent* DashTimelineComponent;

	UPROPERTY(EditAnywhere, Category = "Dash")
	UCurveFloat* TimelineCurve;


	FOnTimelineFloat TimelineTick;

	FOnTimelineEvent TimelineFinish;





public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
