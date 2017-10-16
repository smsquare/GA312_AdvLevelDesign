#pragma once

#include "GameFramework/Actor.h"
#include "BASE_Projectile.generated.h"

UCLASS()
class LEVELCONCEPT_API ABASE_Projectile : public AActor  {
	GENERATED_BODY()

// BASE VARIABLES //
public:
	UPROPERTY(EditAnywhere, Category = "Info")
	FName ProjectileName;
	UPROPERTY(EditAnywhere, Category = "Info")
	float ProjectileLifeSpan; 
	UPROPERTY(EditAnywhere, Category = "Info")
	float ProjectileRateOfFire;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float InitialSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* pCollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* pProjectileMovementComponent;

// BASE METHODS //
public:
	ABASE_Projectile();
	void LaunchProjectile(const FVector& aLaunchDirection);

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

};
