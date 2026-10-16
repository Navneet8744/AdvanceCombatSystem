


#include "Notifies/MN_HeavyCameraShake.h"
#include "Components/CameraSetComponent.h"

void UMN_HeavyCameraShake::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Actor = Cast<AActor>(MeshComp->GetOwner());

	if (!Actor) return;

	CameraComponent = Actor->FindComponentByClass<UCameraSetComponent>();

	if (CameraComponent)
	{
		
		CameraComponent->PlayCameraShake(CameraShakeClass);
	}


}



void UMN_HeavyCameraShake::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (CameraComponent)
	{
		CameraComponent->PauseCameraShake();
	}


}
