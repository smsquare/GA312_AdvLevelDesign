// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Player.h"
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
	PrimaryActorTick.bStartWithTickEnabled = true;
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
	FloorCollider->SetCollisionProfileName(FName("BreakawayFloor"));
	FloorCollider->OnComponentBeginOverlap.AddDynamic(this, &ALD_BreakawayFloor::FloorOverlapDetection);
	SetColliderLocation(FloorSize);
	SetColliderExtents(FloorSize);
}

void ALD_BreakawayFloor::SetColliderExtents(EFloorSize sizeOfFloor) {
	FVector extents;
	switch (sizeOfFloor) {
	case EFloorSize::FS_100:
		break;
	case EFloorSize::FS_250:
		break;
	case EFloorSize::FS_500:
		extents = FVector(245.0f, 250.0f, 17.0f);
		break;
	}
	FloorCollider->SetBoxExtent(extents, true);
}
void ALD_BreakawayFloor::SetColliderLocation(EFloorSize sizeOfFloor) {
	FVector loc;
	
	switch (sizeOfFloor) {
	case EFloorSize::FS_100:
		//loc.X = 
		break;
	case EFloorSize::FS_250:
		break;
	case EFloorSize::FS_500:
		loc = FVector(250.0f, -500.0f, 0.0f);
		break;
	}
	FloorCollider->SetRelativeLocation(loc);
}
// Called when the game starts or when spawned
void ALD_BreakawayFloor::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void ALD_BreakawayFloor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Purple, "TESING IF TICK HAPPENS");
}

void ALD_BreakawayFloor::FloorOverlapDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	ALD_Player* player = Cast<ALD_Player>(OtherActor);
	if (player) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "Player overlapped the floor, it should probably break.");
	}
}