#include "Items/TwinSword/TwinSword.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CollisionComponent.h"
#include "Engine/DataTable.h"
#include "Structs/Structures.h"

#include "Components/MotionWarpComponent.h"
#include "Components/HitImpactComponent.h"



ATwinSword::ATwinSword()
{

	Sword_1 = CreateDefaultSubobject<UStaticMeshComponent>("Sword_1");
	Sword_1->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetRootComponent(Sword_1);
	

	Sword_2 = CreateDefaultSubobject<UStaticMeshComponent>("Sword_2");
	Sword_2->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sword_2->SetupAttachment(Sword_1);

	Sphere->SetupAttachment(Sword_1);


	CollisionComponent_1 = CreateDefaultSubobject<UCollisionComponent>("CollisionComponent_1");
	

	CollisionComponent_2 = CreateDefaultSubobject<UCollisionComponent>("CollisionComponent_2");



	CollisionComponent_1->SetupAttachment(Sword_1);
	CollisionComponent_2->SetupAttachment(Sword_2);
	


}




void ATwinSword::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComponent_1 && CollisionComponent_2)
	{
		CollisionComponent_1->AttachTracePoints(Sword_1);
		CollisionComponent_2->AttachTracePoints(Sword_2);

		CollisionComponent_1->OnHitResponse.AddDynamic(this, &ATwinSword::OnHit);
		CollisionComponent_2->OnHitResponse.AddDynamic(this, &ATwinSword::OnHit);
	}



}







void ATwinSword::OnHit(TArray<FHitResult> HitResults)
{
	Super::OnHit(HitResults);
	
	


}






void ATwinSword::AttachToBody(USkeletalMeshComponent* Mesh)
{


	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);

	Sword_1->AttachToComponent(Mesh, Rules, TwinSwordData.UnEquipSocket_1);

	Sword_2->AttachToComponent(Mesh, Rules, TwinSwordData.UnEquipSocket_2);


	Super::AttachToBody(Mesh);

}




void ATwinSword::AttachToHand(USkeletalMeshComponent* Mesh)
{
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);

	Sword_1->AttachToComponent(Mesh, Rules, TwinSwordData.EquipSocket_1);

	Sword_2->AttachToComponent(Mesh, Rules, TwinSwordData.EquipSocket_2);


	Super::AttachToHand(Mesh);


}



void ATwinSword::CollisionTraceStart()
{

	
	if (!CollisionComponent_1 || !CollisionComponent_2) return;

	Super::CollisionTraceStart();
	
	CollisionComponent_1->ActivateCollision();
	CollisionComponent_2->ActivateCollision();
}




void ATwinSword::CollisionTraceEnd()
{
	if (!CollisionComponent_1 || !CollisionComponent_2) return;

	CollisionComponent_1->DeactivateCollision();
	CollisionComponent_2->DeactivateCollision();
	

}

FAttackData ATwinSword::DefaultAttackSetup(AActor* owner, FCombatInfo CombatInfo, const TObjectPtr<UChooserTable> AttackType)
{
	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	Context.AddObjectParam(owner);
	Context.AddStructParam(CombatInfo);


	FInstancedStruct ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(AttackType);

	UDataTable* AttackInformation = Cast<UDataTable>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UDataTable::StaticClass(), false));

	if (!AttackInformation)  return FAttackData();
		


	static const FString ContextString(TEXT("Attack Lookup"));

	const FAttackData* FoundAttackData = AttackInformation->FindRow<FAttackData>(FName("NewRow"), ContextString);

	if (!FoundAttackData) return FAttackData();

	if (IsValid(MotionWarpComponent) && FoundAttackData->WarpingSetting.bUseWarping && IsValid(CombatInfo.AttackTarget))
	{
		MotionWarpComponent->SetupAttackWarp(CombatInfo.AttackTarget, FoundAttackData->WarpingSetting);
	}

	return *FoundAttackData;

}








void ATwinSword::NormalAttack(FCombatInfo CombatInfo)
{
	if (!IsValid(Character)) return;

	AttackInfo = FAttackData{};

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	

	AttackInfo = DefaultAttackSetup(GetOwner(), CombatInfo, TwinSwordData.NormalAttackChooser);

	if (!AnimInstance || !AttackInfo.AttackMontage) return;

	if (AttackInfo.ExecutionSetting.IsExecutionAttack)
	{

		ExecutionAttack(CombatInfo);
		return;
	}


	

	

	

	if (AttackInfo.LaunchData.bAirLaunchAttack)
	{
		AActor* Attacker = GetOwner();
		AActor* AttackTarget = CombatInfo.AttackTarget;
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this, AttackTarget,Attacker](UAnimMontage* Montage, bool bInterrupted)
			{
				if (bInterrupted) return;

				AirAttackEnded(AttackInfo, AttackTarget,Attacker);
			}
		);


		AnimInstance->Montage_Play(AttackInfo.AttackMontage);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackInfo.AttackMontage);
		return;


	}


	AnimInstance->Montage_Play(AttackInfo.AttackMontage);

	

}

void ATwinSword::HeavyAttackImplement(FCombatInfo CombatInfo)
{
	if (!IsValid(Character)) return;

	AttackInfo = FAttackData{};

	AttackInfo = DefaultAttackSetup(GetOwner(), CombatInfo, TwinSwordData.HeavyAttackImplementChooser);





	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance || !AttackInfo.AttackMontage) return;
	AnimInstance->Montage_Play(AttackInfo.AttackMontage);



}



void ATwinSword::HeavyAttackExecute()
{
	if (!IsValid(Character)) return;

	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	Context.AddObjectParam(GetOwner());
	Context.AddStructParam(AttackInfo);

	FInstancedStruct ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(TwinSwordData.HeavyAttackExecuteChooser);

	UDataTable* AttackInformation = Cast<UDataTable>(UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct, UDataTable::StaticClass(), false));

	if (!AttackInformation)
	{
		CancelHeavyAttack();
		return;
	}

	static const FString ContextString(TEXT("Attack Lookup"));

	const FAttackData* FoundAttackData = AttackInformation->FindRow<FAttackData>(FName("NewRow"), ContextString);

	if (!FoundAttackData || !FoundAttackData->AttackMontage)
	{
		CancelHeavyAttack();
		return;
	}

	AttackInfo = *FoundAttackData;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	AnimInstance->Montage_Play(AttackInfo.AttackMontage);
}



void ATwinSword::ExecutionAttack(FCombatInfo CombatInfo)
{
	AttackInfo = FAttackData{};

	if (!IsValid(Character )) return;

	if (!IsValid(CombatInfo.AttackTarget))
		return;

	AttackInfo = DefaultAttackSetup(GetOwner(), CombatInfo, TwinSwordData.ExecutionChooser);

	if (!AttackInfo.AttackMontage)
		return;

	UHitImpactComponent* HitImpactComponent = CombatInfo.AttackTarget->FindComponentByClass<UHitImpactComponent>();


	if (!IsValid(HitImpactComponent))
		return;

	if (!HitImpactComponent->PlayEnemyExecutionMontage(GetOwner(), AttackInfo)) return;
		

    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();


    if (!IsValid(AnimInstance))
		return;

			FOnMontageEnded EndDelegate;

			EndDelegate.BindUObject(this, &ATwinSword::ExecutionEnded);

			AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackInfo.AttackMontage);
			AnimInstance->Montage_Play(AttackInfo.AttackMontage);

}




void ATwinSword::AirAttack(FCombatInfo CombatInfo)
{
	AttackInfo = FAttackData{};

	if (!IsValid(Character)) return;

	AttackInfo = DefaultAttackSetup(GetOwner(), CombatInfo, TwinSwordData.AirAttackChooser);

	if (!AttackInfo.AttackMontage)
	{
		CancelHeavyAttack();
		return;
	}
	
	AActor* AttackTarget = CombatInfo.AttackTarget;
    AActor* Attacker = GetOwner();

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	AnimInstance->StopAllMontages(0.25);

	FOnMontageEnded EndDelegate;
	
	EndDelegate.BindLambda([this,AttackTarget,Attacker](UAnimMontage* Montage, bool bInterrupted)
		{
			
			if (bInterrupted) return;

			AirAttackEnded(AttackInfo, AttackTarget, Attacker);
		}
	);

	AnimInstance->Montage_Play(AttackInfo.AttackMontage);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackInfo.AttackMontage);


}

void ATwinSword::AirAttackEnded(const FAttackData& AttackData, AActor* TargetActor, AActor* Attacker)
{
	if (!TargetActor) return;
	
		
	
	
	if (UHitImpactComponent* HitImpactComponent = TargetActor->FindComponentByClass<UHitImpactComponent>())
		{
		
		HitImpactComponent->AirAttackEnded(AttackData, Attacker);

		}
	
}
