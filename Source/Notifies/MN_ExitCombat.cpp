// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/MN_ExitCombat.h"
#include "Interfaces/I_DamageAble.h"

void UMN_ExitCombat::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* ownerClass = MeshComp->GetOwner();

	if (IsValid(ownerClass) && ownerClass->Implements<UI_DamageAble>())
	{
		II_DamageAble::Execute_ExitCombat(ownerClass);
		II_DamageAble::Execute_SetActionState(ownerClass, EActionState::EAS_Engaged);
	}

	
}

void UMN_ExitCombat::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* ownerClass = MeshComp->GetOwner();

	if (IsValid(ownerClass) && ownerClass->Implements<UI_DamageAble>())
	{
		II_DamageAble::Execute_SetActionState(ownerClass, EActionState::EAS_UnOccupied);
	}
}
