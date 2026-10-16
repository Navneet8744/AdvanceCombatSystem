// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroundHitDestruction.generated.h"


class USkeletalMeshComponent;
class UAnimSequence;

UCLASS()
class COMBATDAMAGE_API AGroundHitDestruction : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AGroundHitDestruction();

protected:

	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* DestructionMesh;

	UPROPERTY(EditAnywhere)
	UAnimSequence* DestructionAnimation;




public:	

	virtual void Tick(float DeltaTime) override;

};
