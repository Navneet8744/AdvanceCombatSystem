// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/MN_HeavyAttackReady.h"
#include "Player/MyCharacter.h"

void UMN_HeavyAttackReady::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);


	AMyCharacter* Character = Cast<AMyCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->AttackHoldState = EAttackHoldState::EHS_HeavyArmed;
	}



}
