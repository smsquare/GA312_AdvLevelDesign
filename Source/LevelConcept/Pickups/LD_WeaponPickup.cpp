// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_WeaponPickup.h"

ALD_WeaponPickup::ALD_WeaponPickup() {
	PickupType = EPickupType::PT_WEAPON;
	PickupName = "Default Weapon Pickup";
}