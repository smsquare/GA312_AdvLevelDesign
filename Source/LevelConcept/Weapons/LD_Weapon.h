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

USTRUCT()
struct LEVELCONCEPT_API FWeaponStats {
	GENERATED_USTRUCT_BODY()
public:

public:
	FWeaponStats();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Stats")
	FWeaponStats CurrentStats;

public:
	FWeapon();
public:
	void WeaponPickup(EWeaponType weaponPickedUp);
private:
	FString DEBUG_GetEWeaponTypeAsString(EWeaponType enumValue) {
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
		if (!enumPtr) return FString("IVALID");

		return enumPtr->GetEnumNameStringByValue((int64)enumValue);
	}

};