// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Player.h"
#include "LD_WeaponPickup.h"

ALD_WeaponPickup::ALD_WeaponPickup() {
	PickupType = EPickupType::PT_WEAPON;
	PickupName = "Default Weapon Pickup";
	TypeOfWeapon = EWeaponType::WT_INVALID;
}

void ALD_WeaponPickup::PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!HasBeenPickedUp) {
		ALD_Player* player = Cast<ALD_Player>(OtherActor);
		if (player) {
			HasBeenPickedUp = true;
			PickupMesh->SetVisibility(false);
			player->PlayerWeapon.WeaponPickup(TypeOfWeapon);
		}
	}
}

void ALD_WeaponPickup::BeginPlay() {
	if (TypeOfWeapon == EWeaponType::WT_INVALID) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "MUST SET TypeOfWeapon on " + GetActorLabel());
	}
}