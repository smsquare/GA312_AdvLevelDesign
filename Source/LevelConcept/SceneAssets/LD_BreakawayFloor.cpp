// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_BreakawayFloor.h"


// Sets default values
ALD_BreakawayFloor::ALD_BreakawayFloor() {
	// One time init over helpers for loading default meshes or other assets...
	struct FBreakawayStatics {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> defaultFloorMesh;
		FBreakawayStatics() :
			defaultFloorMesh(TEXT("/Game/Geometry/Meshes/Floors/SM_Floor_500x1000.SM_Floor_500x1000")) {
		}
	};
	static FBreakawayStatics BreakawayStatics;

 	// Start with tick turned off.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	// init Settings
	FloorSize = EFloorSize::FS_500;
	TimeToBreakaway = 1.25;
	TimeToRespawn	= 3.0;
	RespawnLocation = GetActorLocation();

	// Setup floor mesh component
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Mesh"));
	FloorMesh->SetStaticMesh(BreakawayStatics.defaultFloorMesh.Get());
	RootComponent = FloorMesh;

	// Setup floor collider;
	FloorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Collider"));
	FloorCollider->AttachToComponent(FloorMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void ALD_BreakawayFloor::SetColliderExtents(EFloorSize) {
	
}

// Called when the game starts or when spawned
void ALD_BreakawayFloor::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void ALD_BreakawayFloor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Purple, "TESING IF TICK HAPPENS");
}

