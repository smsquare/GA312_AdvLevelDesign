#include "LevelConcept.h"
#include "LD_PlayerController.h"
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

void FWeaponStats::SetMaxAmmo(const EWeaponType ammoType) {
	MaxAmmo = ListOf_MaxAmmo[(int)ammoType];
}

/*****************************************************
**													**
**					  FWeapon						**
**													**
*****************************************************/
FWeapon::FWeapon() {
	CurrentWeapon = EWeaponType::WT_DEFAULT;
}

void FWeapon::FireWeapon(const UWorld* world, ALD_PlayerController* playerController, AActor* player) {
	//TODO: If not on cooldown
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "FireWeapon ON FWeapon!!!!");
}

void FWeapon::EquipWeapon(EWeaponType weapon) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "Equiped " + DEBUG_GetEWeaponTypeAsString(weapon));
	
	CurrentProjectile = WeaponStats.ListOf_Projectiles[(int)weapon];
	WeaponStats.SetMaxAmmo(weapon);
}

void FWeapon::WeaponPickup(EWeaponType weaponPickedUp) {
	EquipWeapon(weaponPickedUp);
}