

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChooserFunctionLibrary.h"
#include "Structs/Structures.h"

#include "HitImpactComponent.generated.h"


class ACharacter;
class UCameraSetComponent;
class AGroundHitDestruction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATDAMAGE_API UHitImpactComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UHitImpactComponent();


	bool bCanExecute;
	bool bIsInAir;

	AActor* RawAttacker;

	bool  DoHitReaction(FAttackData& AttackData, bool IsDied);

	bool PlayHitReactMontage(FAttackData& AttackData);

	bool PlayEnemyExecutionMontage(AActor * Attacker, FAttackData AttackData);

	void DisableAttackerAndTargetCollision(ACharacter* Triggerer);
	void EnableAttackerAndTargetCollision(ACharacter* Triggerer);
	
	bool PlayAirAttackReaction(FAttackData& AttackData);

	void SetupAirAttackGravity(AActor* Attacker, AActor* Target, float Gravityindex);


	void AirAttackEnded(const FAttackData& AttackData, AActor* Attacker);

	bool PlayDieReaction(FAttackData& AttackData);

	UFUNCTION()
	void OnHitReactEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	EAttackDirection GetDamageDirection(FVector HitPoint);

	void PlayImpactparticle(FAttackData& AttackData);

protected:
	
	virtual void BeginPlay() override;

	TObjectPtr<ACharacter> Character;


	FAttackData RawAttackData;

	
	UFUNCTION()
	void OnActorLanded(const FHitResult& Hit);

	void DoDestruction(FVector HitPoint, FAttackData  AttackData, FRotator Rotation);
	

	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> HitReactChooser;


	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> DieChooser;


	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> ExecutionChooser;


	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> HitParticleChooser;


	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable>  AirHitReactChooser;


	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> AirHitRecoverChooser;


	UPROPERTY(EditAnywhere, Category = "Chooser")
	TObjectPtr<UChooserTable> DestructingGroundChooser;


	TObjectPtr<UCameraSetComponent> PlayerCameraComponent;
	

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
