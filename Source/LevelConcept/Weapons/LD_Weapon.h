#pragma once

#include "UObject/NoExportTypes.h"
#include "LD_Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	WT_DEFAULT = 0	UMETA(DisplayName = "Default"),
	WT_LASER		UMETA(DisplayName = "Bouncy Laser"),
	WT_GRENADE		UMETA(DisplayName = "Grenade"),
	MAX				UMETA(Hidden)
};

UCLASS()
class LEVELCONCEPT_API ULD_Weapon : public UObject {
	GENERATED_BODY()
public:
	ULD_Weapon();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType CurrentWeapon;
	
};
