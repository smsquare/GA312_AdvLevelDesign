#pragma once

#include "BASE/BASE_ItemPickup.h"
#include "LD_Weapon.h"
#include "LD_WeaponPickup.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_WeaponPickup : public ABASE_ItemPickup {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	EWeaponType TypeOfWeapon;
public:
	ALD_WeaponPickup();	

	void PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};