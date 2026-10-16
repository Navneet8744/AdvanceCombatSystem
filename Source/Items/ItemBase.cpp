


#include "Items/ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CombatComponent.h"
#include "Components/MotionWarpComponent.h"
#include "Player/MyCharacter.h"
#include "GameFramework/Character.h"

AItemBase::AItemBase()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	

	

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	


	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	

}

void AItemBase::Equip(USkeletalMeshComponent* Mesh, AActor* owner, APawn* instigator)
{
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetOwner(owner);
	SetInstigator(instigator);

	if (owner) Character = Cast<ACharacter>(owner);

	MotionWarpComponent = Character->FindComponentByClass<UMotionWarpComponent>();
	
	
}


void AItemBase::UnEquip(USkeletalMeshComponent* Mesh)
{
	

	
}

void AItemBase::AttachToBody(USkeletalMeshComponent* Mesh)
{

}

void AItemBase::AttachToHand(USkeletalMeshComponent* Mesh)
{
}

void AItemBase::UseItem(FCombatInfo CombatInfo)
{
}

void AItemBase::CollisionTraceStart()
{

}


void AItemBase::CollisionTraceEnd()
{

}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	
	

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnSphereOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnSphereOverlapEnd);

	
}


void AItemBase::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor))
	{
		MyCharacter->SetupOverlappingItem(this);
	}

	
}

void AItemBase::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedOComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor))
	{
		MyCharacter->SetupOverlappingItem(nullptr);
	}
}


void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

