#pragma once

#include "CoreMinimal.h"
#include "ENUM.generated.h"









UENUM(BlueprintType)

enum class EEnemyState : uint8
{

	
	EES_Passive UMETA(DisplayName = "Idle"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Investigating UMETA(DisplayName = "Patrolling"),
	EES_Seeking UMETA(DisplayName = "Seeking"),
	EES_Hiding UMETA(DisplayName = "Hiding"),
	EES_Dead UMETA(DisplayName = "Dead")

};


UENUM(BlueprintType)

enum class EAISense : uint8
{
	ES_None UMETA(DisplayName = "None"),
	ES_Sight UMETA(DisplayName = "Sight"),
	ES_Hearing UMETA(DisplayName = "Hearing"),
	ES_Damage UMETA(DisplayName = "Damage")
};




UENUM(BlueprintType)

enum class EWeaponType : uint8

{
	EWT_None  UMETA(DisplayName = "None"),
	EWT_TwinSword  UMETA(DisplayName = "TwinSword"),
	EWT_Projectile UMETA(DisplayName = "Projectile"),
	EWT_Explosion UMETA(DisplayName = "Explosion"),
	EWT_Environment UMETA(DisplayName = "Environment")




};





UENUM(BlueprintType)

enum class EAttackDirection : uint8
{
	EDD_FromFront UMETA(DisplayName = "FromFront"),
	EDD_FromBack UMETA(DisplayName = "FromBack"),
	EDD_FromLeft UMETA(DisplayName = "FromLeft"),
	EDD_FromRight UMETA(DisplayName = "FromRight")

};


UENUM(BlueprintType)

enum class EAttackType : uint8
{
	EAT_Light UMETA(DisplayName = "Light"),
	EAT_Mid UMETA(DisplayName = "Mid"),
	EAT_Heavy UMETA(DisplayName = "Heavy"),
	EAT_InAir UMETA(DisplayName = "InAir"),
};


UENUM(BlueprintType)

enum class EDamageResponse : uint8
{

	EDR_None UMETA(DisplayName = " None"),
	EDR_HitReaction UMETA(DisplayName = "HitReaction"),
	EDR_Stagger UMETA(DisplayName  = "Stagger"),
	EDR_Stan UMETA(DisplayName  = "Stan"),
	EDR_KnockBack UMETA(DisplayName = "KnockBack")
	


};


UENUM(BlueprintType)

enum class EItemType : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIT_TwinSword UMETA(DisplayName = "TwinSword"),
	EIT_Protector UMETA(DisplayName = "Protector")

};



UENUM(BlueprintType)

enum class EActionState : uint8
{

	EAS_Engaged UMETA(DisplayName = "Engaged"),
	EAS_UnOccupied UMETA(DisplayName = "UnOccupied"),
	

};





UENUM(BlueprintType)
enum class EAttackRange : uint8
{

	EAR_CloseRange UMETA(DisplayName = "Close"),
	EAR_MidRange UMETA(DisplayName = "Mid"),
	EAR_FarRange UMETA(DisplayName = "Far"),
	EAR_OutOfRange UMETA(DisplayName = "OutOfRange"),
};




UENUM(BlueprintType)

enum class ERotationMode : uint8
{
	ERM_OrientToCamera UMETA(DisplayName = "OrientToCamera"),
	ERM_OrientToMovement UMETA(DisplayName = "OrientToMovement"),

};

UENUM(BlueprintType)


enum class EWarpPosition : uint8
{
	EWP_None UMETA(DisplayName = "None"),
	EWP_Front UMETA(DisplayName = "Front"),
	EWP_Back UMETA(DisplayName = "Back"),
	EWP_Left UMETA(DisplayName = "Left"),
	EWP_Right UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)

enum class EMovementState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_Walking UMETA(DisplayName = "Walking"),
	EMS_Jogging UMETA(DisplayName = "Jogging"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting")



};


UENUM(BlueprintType)
enum class EAttackHoldState : uint8
{
	EHS_None UMETA(DisplayName = "None"),
	EHS_WaitingForHeavy UMETA(DisplayName = "Waiting For Heavy"),
	EHS_HeavyPose UMETA(DisplayName = "Heavy Pose"),
	EHS_HeavyArmed UMETA(DisplayName = "Heavy Armed")
};




UENUM(BlueprintType)
enum class EAirDamageState : uint8
{
	EAAS_InAir UMETA(DisplayName = "InAir"),
	EAAS_Landing UMETA(DisplayName = "Landing"),
	EAAS_OnGround UMETA(DisplayName = "OnGround")

};