// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelConcept.h"
#include "BASE_ItemPickup.h"

// Sets default values
ABASE_ItemPickup::ABASE_ItemPickup() {
 	PrimaryActorTick.bCanEverTick = false;

	PickupType = EPickupType::PT_INVALID;
	PickupName = "INVALID NAME OF PICKUP";
	PickupRadius = 50.0f;
	HasBeenPickedUp = false;	
	// Setup Pickup Collider as RootComponent
	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Collider"));
	PickupCollider->SetSphereRadius(PickupRadius);
	PickupCollider->SetCollisionProfileName(FName("PickupObject"));
	PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &ABASE_ItemPickup::PickupDetection);
	RootComponent = PickupCollider;
	// Set PickupMesh
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickupMesh->SetVisibility(true);
	PickupMesh->AttachToComponent(PickupCollider, FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void ABASE_ItemPickup::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ABASE_ItemPickup::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABASE_ItemPickup::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) {
	//Get all components
	TArray<UActorComponent*> ownedComponents;
	GetComponents(ownedComponents);

	// Get the name of the property that changed
	FName propertyName = (PropertyChangedEvent.Property != NULL) ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(ABASE_ItemPickup, PickupRadius)) {
		PickupCollider->SetSphereRadius(PickupRadius);
	}

}

void ABASE_ItemPickup::PickupDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
//TODO: Impliment Pickup Detection
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, "BASE_PickupDetection");
}

