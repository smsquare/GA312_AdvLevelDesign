// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Player.h"
#include "LD_SmallKey.h"


// Sets default values
ALD_SmallKey::ALD_SmallKey() {
	PrimaryActorTick.bCanEverTick = true;

	RotationSpeed = -45.0f;

	SmallKeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Small Key Mesh"));
	RootComponent = SmallKeyMesh;
	SmallKeyMesh->OnComponentBeginOverlap.AddDynamic(this, &ALD_SmallKey::PickupDetection);
}

// Called when the game starts or when spawned
void ALD_SmallKey::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ALD_SmallKey::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	FHitResult result;
	FRotator rotationAmount = FRotator(0.0f, RotationSpeed* DeltaTime, 0.0f);
	Super::K2_AddActorLocalRotation(rotationAmount, false, result, false);
}

void ALD_SmallKey::PickupDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	ALD_Player* player = Cast<ALD_Player>(OtherActor);
	if (player) {
		player->PickupSmallKey();
		Destroy();
	}
}