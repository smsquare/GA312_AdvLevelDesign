#include "LevelConcept.h"
#include "BASE_Projectile.h"
#include "LD_Player.h"
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
	RateOfFire = 0.25f;
	// MaxAmmo Database
	ListOf_MaxAmmo[(int)EWT::WT_DEFAULT] = -1;
	ListOf_MaxAmmo[(int)EWT::WT_LASER] = 15;
	ListOf_MaxAmmo[(int)EWT::WT_GRENADE] = 5;
	// RateOfFire Database
	ListOf_RateOfFire[(int)EWT::WT_DEFAULT] = 0.25f;
	ListOf_RateOfFire[(int)EWT::WT_LASER] = 1.0f;
	ListOf_RateOfFire[(int)EWT::WT_GRENADE] = 2.0f;
	// Allocate space for
	ListOf_Projectiles.AddDefaulted((int)EWT::MAX);
}

int FWeaponStats::GetCurrAmmo() const {
	return CurrAmmo;
}

int FWeaponStats::GetMaxAmmo() const {
	return MaxAmmo;
}

float FWeaponStats::GetRateOfFire() const {
	return RateOfFire;
}

TSubclassOf<class ABASE_Projectile> FWeaponStats::GetProjectile(const EWeaponType projectile) const {
	return ListOf_Projectiles[(int)projectile];
}

void FWeaponStats::SetWeaponStats(const EWeaponType weapon) {
	SetCurrAmmo(weapon);
	SetMaxAmmo(weapon);
	SetRateOfFire(weapon);
}

void FWeaponStats::SetCurrAmmo(const EWeaponType ammoType) {
	CurrAmmo = ListOf_MaxAmmo[(int)ammoType];
}

void FWeaponStats::SetMaxAmmo(const EWeaponType ammoType) {
	MaxAmmo = ListOf_MaxAmmo[(int)ammoType];
}

void FWeaponStats::SetRateOfFire(const EWeaponType weapon) {
	RateOfFire = ListOf_RateOfFire[(int)weapon];
}

// Uses a round of the current ammunition
// Returns true if that was the last round fired, false if there is still ammo
bool FWeaponStats::UsedAmmo() {
	bool result = false;
	// Decrease the current ammo
	CurrAmmo -= 1;
	// If there is still ammo remaining
	(CurrAmmo > 0) ? 
		// Then result is false since there is still ammo
		result = false : 
		// if the ammo is out, result is true
		result = true;

	return result;
}

/*****************************************************
**													**
**					  FWeapon						**
**													**
*****************************************************/
FWeapon::FWeapon() {
	CurrentWeapon = EWeaponType::WT_DEFAULT;
	IsShotOnCooldown = false;
}

void FWeapon::FireWeapon(UWorld* world, ALD_PlayerController* playerController, ALD_Player* player) {
	if (!IsShotOnCooldown) {
		// Player is shooting the default weapon
		if (CurrentWeapon == EWeaponType::WT_DEFAULT) {
			ShootProjectile(world, playerController, player);
			StartShotCooldown(player);
			return;
		}
		// Handle special ammo
		else {
			// If player has ammo
			if (WeaponStats.GetCurrAmmo() > 0) {
				ShootProjectile(world, playerController, player);
				StartShotCooldown(player);
				// Use a round of ammo and if you ran out of ammo
				if (WeaponStats.UsedAmmo()) {
					// Equip your default weapon since you ran out of special ammo
					EquipWeapon(EWeaponType::WT_DEFAULT);
				}
			}
		}
	}
}

void FWeapon::WeaponPickup(EWeaponType weaponPickedUp) {
	EquipWeapon(weaponPickedUp);
}

void FWeapon::EquipWeapon(EWeaponType weapon) {
	CurrentWeapon = weapon;
	CurrentProjectile = WeaponStats.GetProjectile(weapon);
	WeaponStats.SetWeaponStats(weapon);
}

void FWeapon::StartShotCooldown(ALD_Player* player) {
	IsShotOnCooldown = true;	
	player->StartFireCooldown(WeaponStats.GetRateOfFire());	
}

void FWeapon::ClearShotCooldown(class ALD_Player* player) {
	
}

void FWeapon::ResetShotCooldown() {
	IsShotOnCooldown = false;
}

void FWeapon::ShootProjectile(UWorld* world, class ALD_PlayerController* playerController, AActor* player) {
	FVector fireDirection = playerController->GetPlayerAimingDirection();
	fireDirection.Normalize();
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner = player;
	spawnParameters.Instigator = player->Instigator;
	spawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn projectile
	ABASE_Projectile* projectile = world->SpawnActor<ABASE_Projectile>(
		(UClass*)CurrentProjectile,
		player->GetActorLocation() + playerController->playerGunLocation,
		fireDirection.ToOrientationRotator(),
		spawnParameters
	);

	// Launch the projectile
	if (projectile) {
		projectile->LaunchProjectile(fireDirection);
	}

	// Play ProjectileFire Sound
	USoundCue* sfx = CurrentProjectile.GetDefaultObject()->ProjectileFireCue;
	if (sfx) {
		UGameplayStatics::PlaySound2D(world, sfx);
	}
}
