// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Player.h"
#include "LD_HealthPackPickup.h"


ALD_HealthPackPickup::ALD_HealthPackPickup() {
	PrimaryActorTick.bCanEverTick = true;
	PickupType = EPickupType::PT_CONSUMABLE;
	PickupName = "Health Pack";
	HealAmount = 10.0f;
	RotationSpeed = 30.0f;
}

void ALD_HealthPackPickup::Tick(float DeltaTime) {
	FHitResult result;
	FRotator rotationAmount = FRotator(0.0f, RotationSpeed* DeltaTime, 0.0f);
	Super::K2_AddActorLocalRotation(rotationAmount, false, result, false);
}

void ALD_HealthPackPickup::PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!HasBeenPickedUp) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, "HealthPack PickupDetection");
		ALD_Player* player = Cast<ALD_Player>(OtherActor);
		if (player) {
			player->HealPlayer(HealAmount);
			PickupMesh->SetVisibility(false);
			HasBeenPickedUp = true;
			ActivateHealParticle();
		}
	}
}