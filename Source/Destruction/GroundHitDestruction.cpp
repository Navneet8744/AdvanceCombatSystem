// Fill out your copyright notice in the Description page of Project Settings.


#include "Destruction/GroundHitDestruction.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AGroundHitDestruction::AGroundHitDestruction()
{
 	
	PrimaryActorTick.bCanEverTick = false;


	DestructionMesh = CreateDefaultSubobject<USkeletalMeshComponent>("DestructionMesh");

	SetRootComponent(DestructionMesh);

	DestructionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void AGroundHitDestruction::BeginPlay()
{
	Super::BeginPlay();


   

    if (DestructionAnimation)
    {
        DestructionMesh->PlayAnimation(
            DestructionAnimation,
            false
        );

        float Duration = DestructionAnimation->GetPlayLength();

        SetLifeSpan(Duration+2.f);
    }
	
}

// Called every frame
void AGroundHitDestruction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

