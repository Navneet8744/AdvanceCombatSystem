

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/Structures.h"
#include "I_DamageAble.generated.h"


UINTERFACE(MinimalAPI)
class UI_DamageAble : public UInterface
{
	GENERATED_BODY()
};


class COMBATDAMAGE_API II_DamageAble
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintNativeEvent)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintNativeEvent)
	float Heal(float HealAmount) ;

	UFUNCTION(BlueprintNativeEvent)
	float TakeDamage(FAttackData& AttackData);


	UFUNCTION(BlueprintNativeEvent)
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateHealthUI(float Percent);

	UFUNCTION(BlueprintNativeEvent)
	void EnterCombat();

	UFUNCTION(BlueprintNativeEvent)
	void ExitCombat();

	UFUNCTION(BlueprintNativeEvent)
	void SetActionState(EActionState State);


	UFUNCTION(BlueprintNativeEvent)
	void CollisionTrace_Start();


	UFUNCTION(BlueprintNativeEvent)
	void CollisionTrace_End();




};
