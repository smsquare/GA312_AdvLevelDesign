#pragma once
#include "EngineMinimal.h"
#include "LD_Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	WT_DEFAULT = 0	UMETA(DisplayName = "Default"),
	WT_LASER		UMETA(DisplayName = "Bouncy Laser"),
	WT_GRENADE		UMETA(DisplayName = "Grenade"),
	MAX				UMETA(Hidden),
	WT_INVALID = 99	UMETA(Hidden, DisplayName = "INVALID")
};

typedef EWeaponType EWT;

USTRUCT()
struct LEVELCONCEPT_API FWeaponStats {
	GENERATED_USTRUCT_BODY()
public:
	// ALL arrays are in the order of the EWeaponType definition.
	// 0.) Default
	// 1.) Laser
	// 2.) Grenade

	// Hook up all of the blueprint implemented weapons.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile")
	TArray<TSubclassOf<class ABASE_Projectile>> ListOf_Projectiles;
	
	UPROPERTY(EditAnywhere, Category = "REFERENCE")
	int ListOf_MaxAmmo[(int)EWT::MAX];

public:
	FWeaponStats();
	void SetMaxAmmo(const EWeaponType ammoType);	

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon|Stats")
	int CurrAmmo;
	UPROPERTY(VisibleAnywhere, Category = "Weapon|Stats")
	int MaxAmmo;
};

USTRUCT()
struct LEVELCONCEPT_API FWeapon {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Projectile")
	TSubclassOf<class ABASE_Projectile> CurrentProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats")
	FWeaponStats WeaponStats;

public:
	FWeapon();
public:
	void FireWeapon(const UWorld* world, class ALD_PlayerController* playerController, AActor* player);
	void EquipWeapon(EWeaponType weapon);
	void WeaponPickup(EWeaponType weaponPickedUp);
private:
	FString DEBUG_GetEWeaponTypeAsString(EWeaponType enumValue) {
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
		if (!enumPtr) return FString("IVALID");

		return enumPtr->GetEnumNameStringByValue((int64)enumValue);
	}
};
