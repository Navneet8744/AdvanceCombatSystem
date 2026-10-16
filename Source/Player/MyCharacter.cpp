// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MyCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "CombatDamage.h"
#include "Components/C_DamageSystem.h"
#include "Components/CombatComponent.h"
#include "Components/TargettingComponent.h"
#include "Components/MotionWarpComponent.h"
#include "Components/DashComponent.h"
#include "Components/TimelineComponent.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetAnimationLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Items/ItemBase.h"
#include "Interfaces/I_EnemyAI.h"
#include "Components/CameraSetComponent.h"

AMyCharacter::AMyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	C_DamageSystem = CreateDefaultSubobject<UC_DamageSystem>("C_DamageSystem");

	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");

	TargettingComponent = CreateDefaultSubobject<UTargettingComponent>("TargettingComponent");

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
	
	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpComponent>("MotionWarpComponent");

	DashComponent = CreateDefaultSubobject<UDashComponent>("DashComponent");


	CameraComponent = CreateDefaultSubobject<UCameraSetComponent>("CameraComponent");


	TeamID = FGenericTeamId(1);
	
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AMyCharacter::RunStart);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this,&AMyCharacter::RunStop);

		EnhancedInputComponent->BindAction(DamageAction, ETriggerEvent::Started, this, &AMyCharacter::DamageTypeChooser);
		EnhancedInputComponent->BindAction(DamageAction, ETriggerEvent::Completed, this, &AMyCharacter::DamageEnemy);

		EnhancedInputComponent->BindAction(Interaction, ETriggerEvent::Started, this, &AMyCharacter::Interact);

		EnhancedInputComponent->BindAction(TargetLockAction, ETriggerEvent::Started, this, &AMyCharacter::TargetLock);

		EnhancedInputComponent->BindAction(LeftTargetLockAction, ETriggerEvent::Started, this, &AMyCharacter::LeftTargetLock);

		EnhancedInputComponent->BindAction(RightTargetLockAction, ETriggerEvent::Started, this, &AMyCharacter::RightTargetLock);
		
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AMyCharacter::Dash);
		
	}
	else
	{
		UE_LOG(LogCombatDamage, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}




void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());


	StimuliSource->RegisterWithPerceptionSystem();

	C_DamageSystem->OnDamageResponse.AddDynamic(this, &AMyCharacter::DamageResponse);
	C_DamageSystem->OnBlockResponse.AddDynamic(this, &AMyCharacter::BlockResponse);
	C_DamageSystem->OnDeadResponse.AddDynamic(this, &AMyCharacter::DeadResponse);

	

	AnimInstance = GetMesh()->GetAnimInstance();

}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAS_Engaged)return;



	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMyCharacter::RunStart()
{
	if(ActionState == EActionState::EAS_Engaged )return;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	IsSprinting = true;
}

void AMyCharacter::RunStop()
{
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	IsSprinting = false;
}



float AMyCharacter::GetMaxHealth_Implementation() const
{
	return C_DamageSystem ? C_DamageSystem->Maxhealth : 0.f;
}

float AMyCharacter::GetCurrentHealth_Implementation() const
{
	return C_DamageSystem ? C_DamageSystem->Health : 0.f;
}

float AMyCharacter::Heal_Implementation(float HealAmount) 
{
	return C_DamageSystem ? C_DamageSystem->Heal(HealAmount) : 0.f;
}

float  AMyCharacter::TakeDamage_Implementation(FAttackData& AttackData) 
{
	return C_DamageSystem->TakeDamage(AttackData);
}

void AMyCharacter::EnterCombat_Implementation()
{
	CurrentItem->AttachToHand(GetMesh());
}

void AMyCharacter::ExitCombat_Implementation()
{
	CurrentItem->AttachToBody(GetMesh());
}

void AMyCharacter::CollisionTrace_Start_Implementation()
{
	if (CombatComponent) CombatComponent->StartCollisionTrace();
}

void AMyCharacter::CollisionTrace_End_Implementation()
{
	if (CombatComponent) CombatComponent->EndCollisionTrace();
}



void AMyCharacter::SetActionState_Implementation(EActionState State)
{
	ActionState = State ;
}




void AMyCharacter::DamageResponse(FAttackData& AttackData)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "DamageResponseCalled");
}

void AMyCharacter::BlockResponse(bool CanBeParried)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "BlockResponseCalled");
}

void AMyCharacter::DeadResponse(FAttackData& AttackData)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "DeadResponseCalled");
}





void AMyCharacter::Interact()
{

	if (ActionState == EActionState::EAS_Engaged) return;

	if (OverlappingItem)
	{
		if (Items.Contains(OverlappingItem)) return;

		if (ItemType == EItemType::EIT_None)
		{
			CurrentItem = OverlappingItem;
			Items.Add(OverlappingItem);
				ItemType = CurrentItem->ItemData.ItemType; 
			CurrentItem->Equip(GetMesh(), this, this);
			
			

			OverlappingItem = nullptr;
			return;

		}


		else
		{
			if (Items.Contains(OverlappingItem)) return;

			Items.Add(OverlappingItem);
			OverlappingItem = nullptr;
			return;
		}
	}


	UpdateItem();


}

void AMyCharacter::UpdateItem()
{
	if (ActionState == EActionState::EAS_Engaged || !CurrentItem) return;
	
	
	if (ItemType == EItemType::EIT_None )
	{
		ItemType = CurrentItem->ItemData.ItemType;
		CurrentItem->Equip(GetMesh(), this, this);
		
		

	}
	else
	{
		CurrentItem->UnEquip(GetMesh());
		ItemType = EItemType::EIT_None;
		TargettingComponent->DisableLockOn();
	}
	
}



void AMyCharacter::Dash()
{	
	if (ActionState == EActionState::EAS_Engaged) return;
	FVector WorldInput = GetCharacterMovement()->GetLastInputVector();

	FVector LocalInput = GetActorTransform().InverseTransformVectorNoScale(WorldInput);

	float DashAngle = 0.f;

	if (!LocalInput.IsNearlyZero())
	{
		DashAngle = FMath::RadiansToDegrees(FMath::Atan2(LocalInput.Y, LocalInput.X));
	}




	if (DashComponent)
	{
		DashComponent->PerformDash(DashAngle);
	}

}

void AMyCharacter::TargetLock()
{
	if (!TargettingComponent)
		return;

	if (TargettingComponent->GetIsTargetting())
	{
		TargettingComponent->DisableLockOn();
	}
	else
	{
		TargettingComponent->ClosestTarget();
	}
}



void AMyCharacter::LeftTargetLock()
{
	if (!TargettingComponent)
		return;

	if (!TargettingComponent->GetIsTargetting())
	{
		TargettingComponent->ClosestTarget();
		return;
	}

	TargettingComponent->ClosestTargetToleft();
}


void AMyCharacter::RightTargetLock()
{
	if (!TargettingComponent)
		return;

	if (!TargettingComponent->GetIsTargetting())
	{
		TargettingComponent->ClosestTarget();
		return;
	}

	TargettingComponent->ClosestTargetToRight();
}







void AMyCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMyCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMyCharacter::DoJumpStart()
{ 
	if (ActionState == EActionState::EAS_Engaged) return;
 	Jump();
	IsJumping = true;
}



void AMyCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
	IsJumping= false;
}





void AMyCharacter::DamageTypeChooser()
{
	if (!CurrentItem || ActionState == EActionState::EAS_Engaged)  return;
		
	AttackHoldState = EAttackHoldState::EHS_WaitingForHeavy;

	GetWorldTimerManager().ClearTimer(AttackTypeTimer);

	GetWorldTimerManager().SetTimer(AttackTypeTimer, FTimerDelegate::CreateLambda([this]()
		{
			if (CombatComponent) CombatComponent->HeavyAttackImplement(GetEnemyInfo());
		}), 0.2f, false);

}



void AMyCharacter::DamageEnemy()
{
	GetWorldTimerManager().ClearTimer(AttackTypeTimer);
	if (!CurrentItem) return;

	
	

	switch (AttackHoldState)
	{
	case EAttackHoldState::EHS_None: CombatComponent->DoAttack(GetEnemyInfo());
		break;
	case EAttackHoldState::EHS_WaitingForHeavy: CombatComponent->DoAttack(GetEnemyInfo());
		break;
	case EAttackHoldState::EHS_HeavyPose: CombatComponent->CancelHeavyAttack();
		break;
	case EAttackHoldState::EHS_HeavyArmed: CombatComponent->HeavyAttackExecute();
		break;
	default:
		break;
	}



}




FCombatInfo AMyCharacter::GetEnemyInfo()
{
	FCombatInfo CombatInfo;

	if (!CombatComponent || !TargettingComponent)
		return CombatInfo;
	AActor* Target = nullptr;

	if (TargettingComponent->GetCurrentTarget())  Target = TargettingComponent->GetCurrentTarget();

	EAttackRange AttackRange = EAttackRange::EAR_OutOfRange;
	EAttackDirection DirectionToAttack = EAttackDirection::EDD_FromFront;

	if (IsValid(Target))
	{
		const float Distance =FVector::Dist(Target->GetActorLocation(),GetActorLocation());

		if (Distance <= 400.f)
		{
			AttackRange = EAttackRange::EAR_CloseRange;
		}
		else if (Distance <= 800.f)
		{
			AttackRange = EAttackRange::EAR_MidRange;
		}
		else if (Distance <= 1500.f)
		{
			AttackRange = EAttackRange::EAR_FarRange;
		}

		CombatInfo.AttackTarget = Target;

		if (Target->Implements<UI_DamageAble>())                        
		{
			CombatInfo.TargetHealth = II_DamageAble::Execute_GetCurrentHealth(Target);

			CombatInfo.CanDoExecution = II_EnemyAI::Execute_CanBeExecute(Target);
			CombatInfo.EnemyState = II_EnemyAI::Execute_GetEnemyState(Target);
			CombatInfo.AirDamageState = II_EnemyAI::Execute_GetAirDamageState(Target);

			if (C_DamageSystem) CombatInfo.AttackDirection = C_DamageSystem->GetEnemyDirection(Target, this);

		}
	}

	CombatInfo.AttackRange = AttackRange;

	if (CombatComponent->ComboCount == 0)
	{
		switch (AttackRange)
		{
		case EAttackRange::EAR_CloseRange:
			DistanceToTarget = 1;
			break;

		case EAttackRange::EAR_MidRange:
			DistanceToTarget = 2;
			break;

		case EAttackRange::EAR_FarRange:
			DistanceToTarget = 3;
			break;

		case EAttackRange::EAR_OutOfRange:
			DistanceToTarget = 0;
			break;

		default:
			break;
		}
	}

	CombatInfo.DistanceToTarget = DistanceToTarget;
	CombatInfo.IsFalling = GetCharacterMovement()->IsFalling();
	CombatInfo.IsBlocking = C_DamageSystem->IsBlocking;
	CombatInfo.IsSprinting = IsSprinting;
	CombatInfo.IsTargetLocked = TargettingComponent->IsTargetting;
	CombatInfo.ComboCount = CombatComponent->ComboCount;

	return CombatInfo;
}





FGenericTeamId AMyCharacter::GetGenericTeamId() const
{
	return TeamID;
}
