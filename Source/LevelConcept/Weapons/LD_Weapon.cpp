// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_Weapon.h"

FWeapon::FWeapon() {
	CurrentWeapon = EWeaponType::WT_DEFAULT;
}

void FWeapon::WeaponPickup(EWeaponType weaponPickedUp) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Pickup of weapon: " + DEBUG_GetEWeaponTypeAsString(weaponPickedUp));
}

