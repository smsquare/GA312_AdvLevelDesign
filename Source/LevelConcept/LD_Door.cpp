// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Player.h"
#include "LD_Door.h"

// Sets default values
ALD_Door::ALD_Door(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsOpen = false;
	IsLocked = false;

	// One time initialization
	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> DoorFrameMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> LeftDoorMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> RightDoorMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> SmallLockMesh;
		FConstructorStatics()
			: DoorFrameMesh(TEXT("/Game/Geometry/Meshes/SceneAssets/SM_Doorframe.SM_Doorframe")),
			LeftDoorMesh(TEXT("/Game/Geometry/Meshes/SceneAssets/SM_Door00L.SM_Door00L")),
			RightDoorMesh(TEXT("/Game/Geometry/Meshes/SceneAssets/SM_Door00R.SM_Door00R")),
			SmallLockMesh(TEXT("/Game/Geometry/Meshes/Locks/STMESH_SmallLock.STMESH_SmallLock")) {
		}
	};
	static FConstructorStatics ConstructorStatics;

	//DoorFrameMesh
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame Mesh"));
	RootComponent = DoorFrameMesh;
	if (ConstructorStatics.DoorFrameMesh.Succeeded()) {
		DoorFrameMesh->SetStaticMesh(ConstructorStatics.DoorFrameMesh.Get());
	}
	DoorFrameMesh->SetVisibility(true);

	//LeftDoorMesh
	LeftDoorMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door Mesh"));
	LeftDoorMesh->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepWorldTransform);
	if (ConstructorStatics.LeftDoorMesh.Succeeded()) {
		LeftDoorMesh->SetStaticMesh(ConstructorStatics.LeftDoorMesh.Get());
	}
	LeftDoorMesh->SetRelativeLocation(FVector(7.0f, -77.0f, 0.0f));
	LeftDoorMesh->SetVisibility(true);
	
	//RightDoorMesh
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door Mesh"));
	RightDoorMesh->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepWorldTransform);
	if (ConstructorStatics.RightDoorMesh.Succeeded()) {
		RightDoorMesh->SetStaticMesh(ConstructorStatics.RightDoorMesh.Get());
	}
	RightDoorMesh->SetRelativeLocation(FVector(7.0f, -77.0f, 0.0f));
	RightDoorMesh->SetVisibility(true);

	//SmallLockMesh
	SmallLockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Small Lock Mesh"));
	SmallLockMesh->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepWorldTransform);
	if (ConstructorStatics.SmallLockMesh.Succeeded()) {
		SmallLockMesh->SetStaticMesh(ConstructorStatics.SmallLockMesh.Get());
	}
	SmallLockMesh->SetWorldLocationAndRotation(
		FVector(-52.0379f, -8.875f, 54.672f), 
		FRotator(-39.567f, 7.644f, -73.533f)
	);
	SmallLockMesh->SetVisibility(false);

	//DoorCollider
	DoorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Door Collider"));
	DoorCollider->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepWorldTransform);
	DoorCollider->SetRelativeLocation(FVector(6.0f, 0.0f, 100.0f));
	DoorCollider->SetBoxExtent(FVector(30.0f, 100.0f, 100.f));
}

void ALD_Door::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	//Get all components
	TArray<UActorComponent*> ownedComponents;
	GetComponents(ownedComponents);

	// Get the name of the property that changed
	FName propertyName = (PropertyChangedEvent.Property != NULL) ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_Door, IsLocked)) {
		SmallLockMesh->SetVisibility(IsLocked);
	}
	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_Door, IsOpen)) {
		if (IsOpen) {
			IsLocked = false;
			SmallLockMesh->SetVisibility(false);
			LeftDoorMesh->SetVisibility(false);
			RightDoorMesh->SetVisibility(false);
			DoorCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DoorCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		}
		else {
			LeftDoorMesh->SetVisibility(true);
			RightDoorMesh->SetVisibility(true);
			DoorCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			DoorCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		}
		
	}
}

bool ALD_Door::GetIsLocked() const {
	return IsLocked;
}

bool ALD_Door::OpenDoor(uint8 numKeys) {
	bool didOpen = false;
	if (!IsOpen) {
		if (numKeys > 0 && IsLocked) {
			IsOpen = true;
			IsLocked = false;
			DoorCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DoorCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			SmallLockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SmallLockMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			SmallLockMesh->SetVisibility(false);
			LeftDoorMesh->SetVisibility(false);
			RightDoorMesh->SetVisibility(false);
			didOpen = true;
		}
		else if (!IsLocked) {
			IsOpen = true;
			DoorCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DoorCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			LeftDoorMesh->SetVisibility(false);
			RightDoorMesh->SetVisibility(false);
			didOpen = true;
		}
	}

	return didOpen;
}

// Called when the game starts or when spawned
void ALD_Door::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALD_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

