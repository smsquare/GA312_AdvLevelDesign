// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Player.h"
#include "LD_HealthUpgradePickup.h"

ALD_HealthUpgradePickup::ALD_HealthUpgradePickup() {
	PrimaryActorTick.bCanEverTick = true;
	
	PickupType = EPickupType::PT_UPGRADE;
	PickupName = "Max Health Upgrade";
	IncreaseHealthAmt = 15.0f;
	HealAmount = 0.0f;
	RotationSpeed = 33.3f;
}

void ALD_HealthUpgradePickup::Tick(float DeltaTime) {
	FHitResult result;
	FRotator rotationAmount = FRotator(0.0f, RotationSpeed* DeltaTime, 0.0f);
	Super::K2_AddActorLocalRotation(rotationAmount, false, result, false);
}
void ALD_HealthUpgradePickup::PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!HasBeenPickedUp) {
		ALD_Player* player = Cast<ALD_Player>(OtherActor);
		if (player && !HasBeenPickedUp) {
			player->HealPlayer(HealAmount);
			player->UpgradeMaxHealth(IncreaseHealthAmt);
			player->PickedUpHealthUpgrade();
			PickupMesh->SetVisibility(false);
			HasBeenPickedUp = true;
			ActivateHealthUpgradeParticle();
		}
	}
}
