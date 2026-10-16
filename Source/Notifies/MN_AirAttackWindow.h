

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "MN_AirAttackWindow.generated.h"


UCLASS()
class COMBATDAMAGE_API UMN_AirAttackWindow : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()


public:

	/*virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;*/
	
};
