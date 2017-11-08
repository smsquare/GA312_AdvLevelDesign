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

float FWeaponStats::GetRateOfFire() const {
	return RateOfFire;
}

void FWeaponStats::SetWeaponStats(const EWeaponType weapon) {
	SetMaxAmmo(weapon);
	SetRateOfFire(weapon);
}

void FWeaponStats::SetMaxAmmo(const EWeaponType ammoType) {
	MaxAmmo = ListOf_MaxAmmo[(int)ammoType];
}

void FWeaponStats::SetRateOfFire(const EWeaponType weapon) {
	RateOfFire = ListOf_RateOfFire[(int)weapon];
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
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "FireWeapon ON FWeapon!!!!");
		// Check current ammo
		// If player has ammo
		ShootProjectile(world, playerController, player);

		StartShotCooldown(player);
	}
}

void FWeapon::WeaponPickup(EWeaponType weaponPickedUp) {
	EquipWeapon(weaponPickedUp);
}

void FWeapon::EquipWeapon(EWeaponType weapon) {
	CurrentProjectile = WeaponStats.ListOf_Projectiles[(int)weapon];
	WeaponStats.SetWeaponStats(weapon);
}

void FWeapon::StartShotCooldown(ALD_Player* player) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "StartShotCooldown!!!!");
	IsShotOnCooldown = true;	
	player->StartFireCooldown(WeaponStats.GetRateOfFire());	
}

void FWeapon::ClearShotCooldown(class ALD_Player* player) {
	
}

void FWeapon::ResetShotCooldown() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "ResetShotCooldown!!!!");
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
}
