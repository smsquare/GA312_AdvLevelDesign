#include "LevelConcept.h"
#include "LD_Weapon.h"

/*****************************************************
**													**
**					FWeaponStats					**
**													**
*****************************************************/
FWeaponStats::FWeaponStats() {
	CurrAmmo = -1;
	MaxAmmo = -1;
	// MaxAmmo Reference
	ListOf_MaxAmmo[(int)EWT::WT_DEFAULT] = -1;
	ListOf_MaxAmmo[(int)EWT::WT_LASER] = 15;
	ListOf_MaxAmmo[(int)EWT::WT_GRENADE] = 5;

	ListOf_Projectiles.AddDefaulted(3);
}

/*****************************************************
**													**
**					  FWeapon						**
**													**
*****************************************************/
FWeapon::FWeapon() {
	CurrentWeapon = EWeaponType::WT_DEFAULT;
}

void FWeapon::WeaponPickup(EWeaponType weaponPickedUp) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Pickup of weapon: " + DEBUG_GetEWeaponTypeAsString(weaponPickedUp));
	// EquipWeapon(weaponPickedUp);
}