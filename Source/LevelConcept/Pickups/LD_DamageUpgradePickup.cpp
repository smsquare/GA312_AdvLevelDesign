// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Player.h"
#include "LD_DamageUpgradePickup.h"

ALD_DamageUpgradePickup::ALD_DamageUpgradePickup() {
	PrimaryActorTick.bCanEverTick = true;

	PickupType = EPickupType::PT_UPGRADE;
	PickupName = "Damage Upgrade";
	IncreaseDamageAmt = 15.0f;
	RotationSpeed = 33.3f;
}

void ALD_DamageUpgradePickup::Tick(float DeltaTime) {
	FHitResult result;
	FRotator rotationAmount = FRotator(0.0f, RotationSpeed* DeltaTime, 0.0f);
	Super::K2_AddActorLocalRotation(rotationAmount, false, result, false);
}

void ALD_DamageUpgradePickup::PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!HasBeenPickedUp) {
		ALD_Player* player = Cast<ALD_Player>(OtherActor);
		if (player && !HasBeenPickedUp) {
			player->UpgradeLBasicDamage(IncreaseDamageAmt);			
			PickupMesh->SetVisibility(false);
			HasBeenPickedUp = true;
			ActivateDamageUpgradeParticle();
		}
	}
}


