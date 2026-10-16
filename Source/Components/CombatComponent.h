
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/Structures.h"
#include "CombatComponent.generated.h"


class AMyCharacter;
class AWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATDAMAGE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	
	UCombatComponent();

	UPROPERTY()
	AMyCharacter* MyCharacter;

	UPROPERTY()
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere)
	int32 ComboCount = 0;




	UPROPERTY()
	FTimerHandle HeavyAttackHandler;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool IsInCombat;


	void DoAttack(FCombatInfo CombatInfo);
	void HeavyAttackImplement(FCombatInfo CombatInfo);

	void CancelHeavyAttack();

    UFUNCTION()
	void HeavyAttackExecute();	

	void ExecutionAttack(FCombatInfo CombatInfo);



	


	UFUNCTION(BlueprintCallable)
	void SaveAttack();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();


	void StartCollisionTrace();
	void EndCollisionTrace();



protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
