// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "MN_Teleport.generated.h"

class AMyCharacter;
UCLASS()
class COMBATDAMAGE_API UMN_Teleport : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

public:

	TObjectPtr<AMyCharacter> Player;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
