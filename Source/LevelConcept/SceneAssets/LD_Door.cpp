// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Player.h"
#include "LD_Door.h"

// Sets default values
ALD_Door::ALD_Door(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	IsOpen = false;
	IsLocked = false;
	KeyNeeded = EKeyColor::KC_INVALID;

	// One time initialization
	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> DoorFrameMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> DoorMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> SmallLockMesh;
		FConstructorStatics()
			: DoorFrameMesh(TEXT("/Game/Geometry/Meshes/Door/TEMP_SM_DoorFrame.TEMP_SM_DoorFrame")),
			DoorMesh(TEXT("/Game/Geometry/Meshes/Door/TEMP_SM_Door.TEMP_SM_Door")),
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

	//DoorMesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepWorldTransform);
	if (ConstructorStatics.DoorMesh.Succeeded()) {
		DoorMesh->SetStaticMesh(ConstructorStatics.DoorMesh.Get());
	}
	//DoorMesh->SetRelativeLocation(FVector(7.0f, -77.0f, 0.0f));
	DoorMesh->SetVisibility(true);
	
	//DoorCollider
	DoorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Door Collider"));
	DoorCollider->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepWorldTransform);
	DoorCollider->SetRelativeLocation(FVector(6.0f, 0.0f, 100.0f));
	DoorCollider->SetBoxExtent(FVector(30.0f, 100.0f, 100.f));
}

void ALD_Door::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//Get all components
	TArray<UActorComponent*> ownedComponents;
	GetComponents(ownedComponents);

	// Get the name of the property that changed
	FName propertyName = (PropertyChangedEvent.Property != NULL) ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_Door, IsOpen)) {
		if (IsOpen) {
			IsLocked = false;
			KeyNeeded = EKeyColor::KC_INVALID;
			//SmallLockMesh->SetVisibility(false);
			DoorCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DoorCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		}
		else {
			DoorCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			DoorCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		}
	}	
	
	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_Door, IsLocked)) {
		if (!IsLocked) {
			KeyNeeded = EKeyColor::KC_INVALID;
		}
	}

}

bool ALD_Door::GetIsLocked() const {
	return IsLocked;
}

bool ALD_Door::OpenDoor() {
	bool didOpen = false;
	if (!IsOpen) {
		if ( IsLocked) {
			IsOpen = true;
			IsLocked = false;
			DoorMesh->DestroyComponent();
			DoorCollider->SetCollisionProfileName(FName("NoCollision"));
			didOpen = true;
		}
		else if (!IsLocked) {
			IsOpen = true;
			DoorMesh->DestroyComponent();
			DoorCollider->SetCollisionProfileName(FName("NoCollision"));
			didOpen = true;
		}
	}
	return didOpen;
}

// Called when the game starts or when spawned
void ALD_Door::BeginPlay() {
	Super::BeginPlay();
	if (IsLocked && KeyNeeded == EKeyColor::KC_INVALID) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(
			-1, 10.0f, FColor::Red, "ERROR: " + GetActorLabel() + ": MUST SET KeyNeeded!");
	}
}

// Called every frame
void ALD_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

