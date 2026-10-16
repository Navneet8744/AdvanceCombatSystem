#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ENUM/ENUM.h"
#include "Structs/Structures.h"
#include "ChooserFunctionLibrary.h"
#include "ItemBase.generated.h"



class USphereComponent;
class UMotionWarpComponent;
class ACharacter;
class UCombatComponent;


USTRUCT(BlueprintType)

struct FItemDeta
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType;

	


	UPROPERTY(EditAnywhere, Category = "Choosers")
	TObjectPtr<UChooserTable> ItemChooser;

	
};

UCLASS()
class COMBATDAMAGE_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItemBase();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FItemDeta ItemData;

	
	 

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;

	UPROPERTY()
	TObjectPtr<ACharacter> Character;



	UPROPERTY()
	TObjectPtr<UMotionWarpComponent> MotionWarpComponent;


	virtual void Equip(USkeletalMeshComponent* Mesh, AActor* owner, APawn* instigator);
	virtual void UnEquip(USkeletalMeshComponent* Mesh);


	virtual void AttachToBody(USkeletalMeshComponent* Mesh);
	virtual void AttachToHand(USkeletalMeshComponent* Mesh);
	


	virtual void UseItem(FCombatInfo CombatInfo);

	virtual void CollisionTraceStart();
	virtual void CollisionTraceEnd();



protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedOComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex);


public:	
	


	virtual void Tick(float DeltaTime) override;

};
