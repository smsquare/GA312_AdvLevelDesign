#include "LevelConcept.h"
#include "LD_Weapon.h"

FWeaponStats::FWeaponStats() {
	CurrAmmo = 0;
	MaxAmmo = 0;
}

FWeapon::FWeapon() {
	CurrentWeapon = EWeaponType::WT_DEFAULT;
}

void FWeapon::WeaponPickup(EWeaponType weaponPickedUp) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Pickup of weapon: " + DEBUG_GetEWeaponTypeAsString(weaponPickedUp));
	// EquipWeapon(weaponPickedUp);
}