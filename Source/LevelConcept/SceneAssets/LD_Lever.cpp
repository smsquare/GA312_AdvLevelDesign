// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Lever.h"


// Sets default values
ALD_Lever::ALD_Lever() {
	PrimaryActorTick.bCanEverTick = false;

	// One time initialization
	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BaseMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> HandleMesh;
		FConstructorStatics()
			: BaseMesh(TEXT("/Game/Geometry/Meshes/Lever/SM_LeverBase.SM_LeverBase")),
			HandleMesh(TEXT("/Game/Geometry/Meshes/Lever/SM_LeverHandle.SM_LeverHandle")) {
		}
	};
	static FConstructorStatics ConstructorStatics;

	HasBeenPulled = false;

	//LeverBaseMash
	LeverBaseMash = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever Base Mesh"));
	RootComponent = LeverBaseMash;
	LeverBaseMash->SetStaticMesh(ConstructorStatics.BaseMesh.Get());
	LeverBaseMash->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeverBaseMash->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	//LeverHandleMesh
	LeverHandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever Handle Mesh"));
	LeverHandleMesh->AttachToComponent(LeverBaseMash, FAttachmentTransformRules::KeepWorldTransform);
	LeverHandleMesh->SetStaticMesh(ConstructorStatics.HandleMesh.Get());
	LeverHandleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeverHandleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	//BoxCollider
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->AttachToComponent(LeverBaseMash, FAttachmentTransformRules::KeepWorldTransform);
	BoxCollider->SetBoxExtent(FVector(10.0f));
	BoxCollider->SetRelativeLocation(FVector(-20.0f, 0.0f, 10.0f));
	LeverHandleMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeverHandleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LeverHandleMesh->bGenerateOverlapEvents = true;
}

//void ALD_Lever::LeverDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
//
//}

// Called when the game starts or when spawned
void ALD_Lever::BeginPlay() {
	Super::BeginPlay();
}
