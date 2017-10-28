// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_BreakawayFloor.h"


// Sets default values
ALD_BreakawayFloor::ALD_BreakawayFloor() {
	//TODO: IMPLEMENT CONSTRUCTOR HELPERS
	/// One time init over helpers for loading default meshes or other assets...
	//struct FConstructorStatics {
	//
	//};
	//

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// init Settings
	TimeToBreakaway = 1.25;
	TimeToRespawn	= 3.0;
	RespawnLocation = GetActorLocation();
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

