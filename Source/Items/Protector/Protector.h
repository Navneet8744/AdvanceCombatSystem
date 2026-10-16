// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WeaponBase.h"
#include "Structs/Structures.h"
#include "ChooserFunctionLibrary.h"
#include "Protector.generated.h"


class UCollisionComponent;
class UStaticMeshComponent;
class UHitImpactComponent;

USTRUCT(BlueprintType)

struct FProtectorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Sockets")
	FName  EquipSocket;

	UPROPERTY(EditAnywhere, Category = "Sockets")
	FName UnEquipSocket;

	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> AttackChooser;


	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> EnemyExecutionChooser;
};





UCLASS()
class COMBATDAMAGE_API AProtector : public AWeaponBase
{
	GENERATED_BODY()

public:

	AProtector();


	UPROPERTY(EditAnywhere, Category = "Protector Data")
	FProtectorData ProtectorData;


	virtual void OnHit(TArray<FHitResult> HitResults) override;


	virtual void AttachToBody(USkeletalMeshComponent* Mesh) override;
	virtual void AttachToHand(USkeletalMeshComponent* Mesh) override;



	virtual void UseItem(FCombatInfo CombatInfo) override;

	virtual void CollisionTraceStart() override;
	virtual void CollisionTraceEnd() override;




protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Protector;


	UPROPERTY(EditAnywhere, Category = "Components")
	UCollisionComponent* CollisionComponent;


	TObjectPtr<UHitImpactComponent> HitImpactComponent;




	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;






		
};
