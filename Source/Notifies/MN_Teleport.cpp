// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/MN_Teleport.h"
#include "Player/MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

void UMN_Teleport::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Player = Cast<AMyCharacter>(MeshComp->GetOwner());

	if (Player)
	{
		
		if (Player->TeleportBeginParticle)
		{
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Player->TeleportBeginParticle, Player->GetMesh()->GetSocketLocation("FX_Teleport"), FRotator::ZeroRotator, true);
		}
		Player->GetMesh()->SetVisibility(false,true);
		
		Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	
	}


}



void UMN_Teleport::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Player)
	{
		Player->GetMesh()->SetVisibility(true, true);

		Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		if (Player->TeleportEndParticle)
		{
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Player->TeleportEndParticle, Player->GetMesh()->GetSocketLocation("FX_Teleport"), FRotator::ZeroRotator, true);
		}
	}
}
