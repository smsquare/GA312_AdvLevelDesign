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
	PrimaryActorTick.bStartWithTickEnabled = false;
	//PrimaryActorTick.SetTickFunctionEnable(true);
	// init Settings
	FloorSize = EFloorSize::FS_500;
	TimeToBreakaway = 1.25;
	TimeToRespawn	= 3.0;
	RespawnLocation = GetActorLocation();
	IsTriggered = false;

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

bool ALD_BreakawayFloor::GetIsTriggered() const {
	return IsTriggered;
}

const UWorld* ALD_BreakawayFloor::GetWorldPtr() const {
	return WorldPtr;
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



void ALD_BreakawayFloor::PostInitializeComponents() {
	Super::PostInitializeComponents();

	WorldPtr = GetOuter()->GetWorld();
	if (WorldPtr == nullptr) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "ERROR: BreakawayFloor WorldPtr initilization FAILED!");
	}
}

// Called every frame
void ALD_BreakawayFloor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ALD_BreakawayFloor::FloorOverlapDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	if (!IsTriggered) {
		ALD_Player* player = Cast<ALD_Player>(OtherActor);
		if (player) {
			IsTriggered = true;
			StartTimerToBreakaway();
		}
	}
}

/**************************************************************** 
						PRIVATE METHODS
*****************************************************************/
void ALD_BreakawayFloor::Breakaway() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(
		-1, 3.0f, FColor::Purple, "Breakaway!!!!"
	);
	FloorCollider->SetCollisionProfileName(FName("NoCollision"));
	FloorMesh->SetCollisionProfileName(FName("NoCollision"));
	FloorMesh->SetVisibility(false);

	StartTimerToRespawn();
}

void ALD_BreakawayFloor::Respawn() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(
		-1, 3.0f, FColor::Cyan, "Respawn!!!!"
	);

	IsTriggered = false;
	FloorMesh->SetVisibility(true);
}

void ALD_BreakawayFloor::StartTimerToBreakaway() {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(
			BreakawayTimer, this, &ALD_BreakawayFloor::Breakaway, TimeToBreakaway, false
		);
	}
}

void ALD_BreakawayFloor::StartTimerToRespawn() {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(
			BreakawayTimer, this, &ALD_BreakawayFloor::Respawn, TimeToBreakaway, false
		);
	}
}