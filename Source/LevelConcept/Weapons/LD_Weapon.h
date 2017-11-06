#pragma once
#include "EngineMinimal.h"
#include "LD_Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	WT_INVALID = 0	UMETA(Hidden, DisplayName = "INVALID"),
	WT_DEFAULT		UMETA(DisplayName = "Default"),
	WT_LASER		UMETA(DisplayName = "Bouncy Laser"),
	WT_GRENADE		UMETA(DisplayName = "Grenade"),
	MAX				UMETA(Hidden)
};

USTRUCT()
struct LEVELCONCEPT_API FWeapon {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType CurrentWeapon;
public:
	FWeapon();

	void WeaponPickup();
};