// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "BASE_Projectile.h"


// Sets default values
ABASE_Projectile::ABASE_Projectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a SphereComponent for projectile collider
	pCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	pCollisionComponent->InitSphereRadius(15.0f);
	RootComponent = pCollisionComponent;

	// Create the ProjectileMovementComponent
	// Use this component to handle movement for the projectile
	pProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	pProjectileMovementComponent->SetUpdatedComponent(pCollisionComponent);
	pProjectileMovementComponent->InitialSpeed = 500.0f;
	pProjectileMovementComponent->MaxSpeed = 500.0f;
	pProjectileMovementComponent->bRotationFollowsVelocity = true;
	pProjectileMovementComponent->bShouldBounce = false;
	pProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// Lock movement along the Y-axis
	pProjectileMovementComponent->bConstrainToPlane = true;
	pProjectileMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

	// Projectile Information
	ProjectileName = "_UNKNOWN_PROJECTILE_";
}

// Called when the game starts or when spawned
void ABASE_Projectile::BeginPlay() {
	Super::BeginPlay();	
}

// Called every frame
void ABASE_Projectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}