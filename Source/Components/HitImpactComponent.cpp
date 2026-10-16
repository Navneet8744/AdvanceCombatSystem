


#include "Components/HitImpactComponent.h"
#include  "Components/CapsuleComponent.h"
#include "Components/CameraSetComponent.h"
#include "Components/TargettingComponent.h"
#include "particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/I_DamageAble.h"
#include "Interfaces/I_EnemyAI.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Destruction/GroundHitDestruction.h"

UHitImpactComponent::UHitImpactComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}

bool UHitImpactComponent::DoHitReaction(FAttackData& AttackData, bool IsDied)
{


	if (!Character)
	{
		Character = Cast<ACharacter>(GetOwner());
	}

	if (!Character)
	{
		return false;
	}



	FAttackData AttackDataa = AttackData;

	RawAttackData = FAttackData{};
	RawAttackData = AttackData;

	AttackDataa.DamageDirection = GetDamageDirection(AttackData.HitPoint);


	if (IsDied)
	{
		return PlayDieReaction(AttackDataa);
	}

	else if(AttackDataa.LaunchData.IsAirAttack)
	{
		
		return PlayAirAttackReaction(AttackDataa);

	}

	else
	{
		return PlayHitReactMontage(AttackDataa);
	}
	
}


void UHitImpactComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		Character = Cast<ACharacter>(GetOwner());
	}

	if (Character)
	{
		PlayerCameraComponent = Character->FindComponentByClass<UCameraSetComponent>();
		Character->LandedDelegate.AddDynamic(this, &UHitImpactComponent::OnActorLanded);
	}

	
}

void UHitImpactComponent::OnActorLanded(const FHitResult& Hit)
{


	SetupAirAttackGravity(RawAttacker, Character, 1.f);
	PlayImpactparticle(RawAttackData);
	if (PlayerCameraComponent) PlayerCameraComponent->DoCameraShake(RawAttackData);

	if (Character->Implements<UI_DamageAble>())
	{
		if (II_DamageAble::Execute_IsDead(Character))
		{
			return;
		}
	}


	if (!RawAttacker) return;

	if (!PlayerCameraComponent)
	{
		PlayerCameraComponent = Character->FindComponentByClass<UCameraSetComponent>();
	}

	

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;
	

	    DoDestruction(Hit.ImpactPoint, RawAttackData, Character->GetActorRotation());	
	    
		
		

		if (Character->Implements<UI_EnemyAI>())
		{
			II_EnemyAI::Execute_SetAirDamageState(Character, EAirDamageState::EAAS_OnGround);
		}


		

	   FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	   Context.AddObjectParam(Character);
	   Context.AddStructParam(RawAttackData);

	  
	 

	   FInstancedStruct ChooserStruct;
	   
	   ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(AirHitRecoverChooser);


	   UAnimMontage* AirHitRecoverMontage = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UAnimMontage::StaticClass(), false));

	   if (!AirHitRecoverMontage) return;


	   
		    
	   AnimInstance->StopAllMontages(0.3);

	   AnimInstance->Montage_Play(AirHitRecoverMontage);
	 
}






void UHitImpactComponent::DoDestruction(FVector HitPoint, FAttackData  AttackData, FRotator Rotation)
{
	FChooserEvaluationContext DestructionContext = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	DestructionContext.AddObjectParam(Character);
	DestructionContext.AddStructParam(AttackData);




	FInstancedStruct DestructionChooserStruct;

	DestructionChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(DestructingGroundChooser);


	UObject* Result = UChooserFunctionLibrary::EvaluateObjectChooserBase(DestructionContext, DestructionChooserStruct, AActor::StaticClass(), true);

	UClass* SelectedClass = Cast<UClass>(Result);

	TSubclassOf<AGroundHitDestruction> GroundDestruction;

	if (SelectedClass && SelectedClass->IsChildOf(AGroundHitDestruction::StaticClass()))
	{
		GroundDestruction = SelectedClass;
	}


	HitPoint.Z = 7.f;

	FActorSpawnParameters Params;
	Params.Owner = Character;
	Params.Instigator = Character;


	GetWorld()->SpawnActor<AGroundHitDestruction>(
		GroundDestruction,
		HitPoint,
		Rotation,
		Params
	);
}

bool UHitImpactComponent::PlayHitReactMontage(FAttackData& AttackData)
{


	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return false;


	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	
	Context.AddObjectParam(Character);
	Context.AddStructParam(AttackData);

	FInstancedStruct ChooserStruct;

	
	
	ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(HitReactChooser);
	
	
	
		UAnimMontage* HitReactMontage = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UAnimMontage::StaticClass(), false));

		if (HitReactMontage)
		{
			if (AttackData.ShouldForceInterrupt)
			{
				AnimInstance->StopAllMontages(0.3);
			}



			if (!PlayerCameraComponent)
			{
				PlayerCameraComponent = Character->FindComponentByClass<UCameraSetComponent>();
		    }
			if (PlayerCameraComponent) PlayerCameraComponent->DoCameraShake(AttackData);

			FOnMontageEnded EndDelegate;

			EndDelegate.BindUObject(this, &UHitImpactComponent::OnHitReactEnded);


			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, HitReactMontage);
			PlayImpactparticle(AttackData);
			
			return true;
		}
		
	

	return false;
}







bool UHitImpactComponent::PlayEnemyExecutionMontage(AActor* Attacker, FAttackData AttackData)
{
	ACharacter* Triggerer = Cast<ACharacter>(Attacker);

	if (!Character || !Triggerer) return false;

	

	FChooserEvaluationContext  Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	Context.AddObjectParam(Character);
	Context.AddStructParam(AttackData);

	FInstancedStruct ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(ExecutionChooser);

	UAnimMontage* AnimMontage = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UAnimMontage::StaticClass(), false));

	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
	{
		UTargettingComponent* TargettingComponent = Triggerer->FindComponentByClass<UTargettingComponent>();

		if (TargettingComponent)
		{
			TargettingComponent->DisableLockOn();
		}

		
		

		DisableAttackerAndTargetCollision(Triggerer);

		FRotator EnemyRotation = Character->GetActorRotation();
		
		

		FRotator PlayerExecutionRotation = EnemyRotation + AttackData.ExecutionSetting.ExecutionRotation;
			

		Triggerer->SetActorRotation(PlayerExecutionRotation);

	

		AnimInstance->StopAllMontages(0.2f);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this, Attacker,TargettingComponent](UAnimMontage* Montage, bool bInterrupted)
			{
				if (Character->Implements<UI_DamageAble>())
				{
					if (!II_DamageAble::Execute_IsDead(Character))
					{
						ACharacter* Triggerer = Cast<ACharacter>(Attacker);
						EnableAttackerAndTargetCollision(Triggerer);

						if (TargettingComponent)
						{
							TargettingComponent->ClosestTarget();
						}
					}
				}
				
			}
		);
		
		
		AnimInstance->Montage_Play(AnimMontage);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AnimMontage);

		
		return true;
	}

	return false;
}

void UHitImpactComponent::DisableAttackerAndTargetCollision(ACharacter* Triggerer)
{
	auto LoggCollision = [](ACharacter* Actor)
		{
			if (!IsValid(Actor))
			{
				return;
			}

			UE_LOG(LogTemp, Warning,
				TEXT("[%s] Capsule: %s | Mesh: %s"),
				*GetNameSafe(Actor),
				*UEnum::GetValueAsString(
					Actor->GetCapsuleComponent()->GetCollisionEnabled()),
				*UEnum::GetValueAsString(
					Actor->GetMesh()->GetCollisionEnabled()));
		};


	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Triggerer->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Triggerer->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


	TArray<AActor*> Enemies;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(), AEnemy::StaticClass(), Enemies);

	UE_LOG(LogTemp, Warning, TEXT("=== BEFORE ==="));

	for (AActor* Actor : Enemies)
	{
		LoggCollision(Cast<ACharacter>(Actor));
	}

	// Your existing collision-disabling code goes here.

	UE_LOG(LogTemp, Warning, TEXT("=== AFTER ==="));

	for (AActor* Actor : Enemies)
	{
		LoggCollision(Cast<ACharacter>(Actor));
	}


}

void UHitImpactComponent::EnableAttackerAndTargetCollision(ACharacter* Triggerer)
{
Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
Triggerer->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
Triggerer->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}







bool UHitImpactComponent::PlayAirAttackReaction(FAttackData& AttackData)
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return false;


	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();


	Context.AddObjectParam(Character);
	Context.AddStructParam(AttackData);

	FInstancedStruct ChooserStruct;



	ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(AirHitReactChooser);


	UAnimMontage* AirReactMontage = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UAnimMontage::StaticClass(), false));
	

	if (!AirReactMontage)
	{
	
		return false;
	}

	

	

	if (!PlayerCameraComponent)
	{
		PlayerCameraComponent = Character->FindComponentByClass<UCameraSetComponent>();
	}
	if (PlayerCameraComponent) PlayerCameraComponent->DoCameraShake(AttackData);

	if (AttackData.LaunchData.bAirLaunchAttack)
	{


		FVector Target_LaunchDirection = (Character->GetActorLocation() - AttackData.AttackingActor->GetActorLocation()).GetSafeNormal();

		Target_LaunchDirection.Z = 0.f;



		if (!Target_LaunchDirection.Normalize())   // returns false if too small
		{
			Target_LaunchDirection = AttackData.AttackingActor->GetActorForwardVector();
			Target_LaunchDirection.Z = 0.f;
			Target_LaunchDirection.Normalize();
		}
		


		FVector Target_LaunchVelocity = Target_LaunchDirection * AttackData.LaunchData.Target_HorizontalLaunchStrength;

		Target_LaunchVelocity.Z = AttackData.LaunchData.Target_VerticalLaunchStrength;




		FVector Attacker_LaunchDirection = -Target_LaunchDirection;

		
		FVector Attacker_LaunchVelocity = Attacker_LaunchDirection * AttackData.LaunchData.Attacker_HorizontalLaunchStrength;

		Attacker_LaunchVelocity.Z = AttackData.LaunchData.Attacker_VerticalLaunchStrength;

		ACharacter* Attacker = Cast<ACharacter>(AttackData.AttackingActor);


		if (Attacker)
		{
			Attacker->LaunchCharacter(Attacker_LaunchVelocity, true, true);
			Character->LaunchCharacter(Target_LaunchVelocity, true, true);

			
		
			SetupAirAttackGravity(AttackData.AttackingActor,Character,0.2f);

			if (Character->Implements<UI_EnemyAI>())
			{
				II_EnemyAI::Execute_SetAirDamageState(Character,EAirDamageState::EAAS_InAir);
			}
			RawAttacker = AttackData.AttackingActor;
		}

	}

	
		

		if (AttackData.ShouldForceInterrupt)
		{
			AnimInstance->StopAllMontages(0.3f);
			
			AnimInstance->Montage_Play(AirReactMontage);
		}
		
		PlayImpactparticle(AttackData);
		
		return true;
	
	
	
}






void UHitImpactComponent::SetupAirAttackGravity(AActor* Attacker, AActor* Target, float GravityIndex)
{
	
		if (Target)
			if (auto* M = Target->FindComponentByClass<UCharacterMovementComponent>())
				M->GravityScale = GravityIndex;

		if (Attacker)
			if (auto* M = Attacker->FindComponentByClass<UCharacterMovementComponent>())
				M->GravityScale = GravityIndex;
	
	
		
}


void UHitImpactComponent::AirAttackEnded(const FAttackData& AttackData, AActor* Attacker)
{
	if (!Attacker)
	{
		return;
	}
	SetupAirAttackGravity(Attacker, Character, 20.f);

	
	
	
}
	


bool UHitImpactComponent::PlayDieReaction(FAttackData& AttackData)
{

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return false;

	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	Context.AddObjectParam(Character);
	Context.AddStructParam(AttackData);

	FInstancedStruct ChooserStruct;


	ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(DieChooser);


	UAnimMontage* DieMontage = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UAnimMontage::StaticClass(), false));


	if (DieMontage)
	{
		if (AttackData.ShouldForceInterrupt)
		{
			AnimInstance->StopAllMontages(0.3);
		}


		if (!PlayerCameraComponent)
		{
			PlayerCameraComponent = Character->FindComponentByClass<UCameraSetComponent>();
		}
		if (PlayerCameraComponent) PlayerCameraComponent->DoCameraShake(AttackData);

		FOnMontageEnded EndDelegate;

		EndDelegate.BindUObject(this, &UHitImpactComponent::OnHitReactEnded);


		AnimInstance->Montage_Play(DieMontage);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DieMontage);
		PlayImpactparticle(AttackData);
		return true;
	}
	return false;
}



void UHitImpactComponent::OnHitReactEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (!bInterrupted) return;
	if (Character->Implements<UI_DamageAble>() && AnimMontage)
	{
		II_DamageAble::Execute_SetActionState(Character, EActionState::EAS_Engaged);
		
	}
}




void UHitImpactComponent::PlayImpactparticle(FAttackData& AttackData)
{
	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	Context.AddObjectParam(Character);
	Context.AddStructParam(AttackData);


	FInstancedStruct ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(HitParticleChooser);

	UParticleSystem* HitParticle = Cast<UParticleSystem>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UParticleSystem::StaticClass(), false));
	
	if (!HitParticle)
	{
		

		return;
	}
	

	USceneComponent* AttachComp = nullptr;
	if (ACharacter* EnemyChar = Cast<ACharacter>(GetOwner()))
	{
		AttachComp = EnemyChar->GetMesh();
	}
	else if (AActor* OwnerActor = GetOwner())
	{
		AttachComp = OwnerActor->GetRootComponent();
	}

	if (AttachComp)
	{
		
		UGameplayStatics::SpawnEmitterAttached(
			HitParticle,
			AttachComp,
			NAME_None,                                  // Optional: Sockets like "spine_03" or NAME_None
			AttackData.HitPoint,                        // World impact point
			FRotator::ZeroRotator,                      // Orientation (or DamageInfo normal rotation)
			EAttachLocation::KeepWorldPosition,         // Keeps the particle at the exact impact contact point
			true                                        // Auto-destroy when finished
		);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			Character->GetActorLocation(),
			FRotator(0.f, 0.f, 0.f),
			true
		);
	}
	


}





EAttackDirection UHitImpactComponent::GetDamageDirection(FVector HitPoint)
{
	if (!Character)
	{
		Character = Cast<ACharacter>(GetOwner());
	}

	if (!Character)
	{
		return EAttackDirection::EDD_FromFront;
	}

	const FVector CharacterLocation = Character->GetActorLocation();

	// Ignore vertical difference.
	FVector HitDirection = HitPoint - CharacterLocation;
	HitDirection.Z = 0.f;
	HitDirection.Normalize();

	const FVector Forward = Character->GetActorForwardVector();

	// Get angle between forward and hit direction.
	float CosTheta = FVector::DotProduct(Forward, HitDirection);
	CosTheta = FMath::Clamp(CosTheta, -1.f, 1.f);

	float Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

	// Determine left/right using cross product.
	const FVector CrossProduct = FVector::CrossProduct(Forward, HitDirection);

	if (CrossProduct.Z < 0.f)
	{
		Theta *= -1.f;
	}

	if (Theta >= -45.f && Theta <= 45.f)
	{
		return EAttackDirection::EDD_FromFront;
	}
	else if (Theta > 45.f && Theta <= 135.f)
	{
		return EAttackDirection::EDD_FromRight;
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		return EAttackDirection::EDD_FromLeft;
	}
	else
	{
		return EAttackDirection::EDD_FromBack;
	}
}


void UHitImpactComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

