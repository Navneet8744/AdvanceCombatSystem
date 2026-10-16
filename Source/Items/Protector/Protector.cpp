


#include "Items/Protector/Protector.h"
#include "Components/CollisionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/HitImpactComponent.h"

AProtector::AProtector()
{
	Protector = CreateDefaultSubobject<UStaticMeshComponent>("Protector");
	Protector->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetRootComponent(Protector);

	CollisionComponent = CreateDefaultSubobject<UCollisionComponent>("CollisionComponent");
	CollisionComponent->SetupAttachment(Protector);




}






void AProtector::OnHit(TArray<FHitResult> HitResults)
{
	Super::OnHit(HitResults);
}




void AProtector::AttachToBody(USkeletalMeshComponent* Mesh)
{
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);

	if (Protector)
	{
		Protector->AttachToComponent(Mesh, Rules, ProtectorData.UnEquipSocket);
	}

	Super::AttachToBody(Mesh);

}




void AProtector::AttachToHand(USkeletalMeshComponent* Mesh)
{

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);


	if (Protector)
	{
		Protector->AttachToComponent(Mesh, Rules, ProtectorData.EquipSocket);
	}

	Super::AttachToHand(Mesh);
}





void AProtector::UseItem(FCombatInfo CombatInfo)
{




}





void AProtector::CollisionTraceStart()
{
	if (!CollisionComponent) return;

	Super::CollisionTraceStart();
	
	CollisionComponent->ActivateCollision();
}





void AProtector::CollisionTraceEnd()
{
	if (!CollisionComponent) return;
	CollisionComponent->DeactivateCollision();
}



void AProtector::BeginPlay()
{

	if (CollisionComponent)
	{
		CollisionComponent->AttachTracePoints(Protector);


		CollisionComponent->OnHitResponse.AddDynamic(this, &AProtector::OnHit);
	}
}

void AProtector::Tick(float DeltaTime)
{
}


