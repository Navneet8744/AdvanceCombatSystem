


#include "Components/CameraSetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HitImpactComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Character.h"


UCameraSetComponent::UCameraSetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UCameraSetComponent::BeginPlay()
{
    Super::BeginPlay();

    CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);

    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (Character)
    {
      HitImpactComponent =   Character->FindComponentByClass<UHitImpactComponent>();
    }
}

void UCameraSetComponent::DoCameraShake(FAttackData& AttackData)
{
    if (!CameraManager || !CameraShakeChooser)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CameraManagerInvalid");
        return;
    }






    FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();



    Context.AddObjectParam(AttackData.AttackingActor);
    Context.AddStructParam(AttackData);

    FInstancedStruct ChooserStruct = UChooserFunctionLibrary::MakeEvaluateChooser(CameraShakeChooser);

    // If the chooser outputs a UClass asset, filter against UClass::StaticClass()
    UObject* EvaluatedObject = UChooserFunctionLibrary::EvaluateObjectChooserBase(Context, ChooserStruct,UCameraShakeBase::StaticClass(), true);

    if (UClass* ShakeClass = Cast<UClass>(EvaluatedObject))
    {
        if (ShakeClass->IsChildOf(UCameraShakeBase::StaticClass()))
        {
            
            CameraManager->StartCameraShake(ShakeClass, 1.0f);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ShakeClassInvalid");
    }
}

void UCameraSetComponent::PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass)
{
    if (CameraManager && CameraShakeClass)
    {
       
        RawShakeClass = CameraManager->StartCameraShake(CameraShakeClass, 1.f);
    }
}

void UCameraSetComponent::PauseCameraShake()
{
    if (CameraManager  && RawShakeClass)
    {
        CameraManager->StopCameraShake(RawShakeClass,false);
    }

}




void UCameraSetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

