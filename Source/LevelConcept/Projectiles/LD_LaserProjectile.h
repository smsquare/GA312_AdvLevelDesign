#pragma once

#include "LevelConcept/BASE/BASE_Projectile.h"
#include "LD_LaserProjectile.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_LaserProjectile : public ABASE_Projectile {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Laser|Bounce")
	uint8 NumBounces;

public:
	ALD_LaserProjectile();

	UFUNCTION(Category = "Proximity")
	void ProjectileCollisionDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};