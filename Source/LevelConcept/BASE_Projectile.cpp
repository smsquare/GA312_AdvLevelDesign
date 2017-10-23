#include "LevelConcept.h"
#include "BASE_Projectile.h"

// Sets default values
ABASE_Projectile::ABASE_Projectile() {
 	PrimaryActorTick.bCanEverTick = true;

	ProjectileOwner = EProjectileOwner::PO_INVALID;
	ProjectileName = "_UNKNOWN_PROJECTILE_";
	ProjectileDamage = 1.0f;
	ProjectileLifeSpan = 3.0f;
	ProjectileRateOfFire = 1.0f;
	
	// Create a SphereComponent for projectile collider
	pCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	pCollisionComponent->InitSphereRadius(15.0f);
	pCollisionComponent->SetCollisionProfileName(FName("PlayerProjectile"));
	RootComponent = pCollisionComponent;
	// Mesh of the actual projectile
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->AttachToComponent(pCollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// Create the ProjectileMovementComponent
	// Use this component to handle movement for the projectile
	pProjectileMovementComponent = 
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	pProjectileMovementComponent->SetUpdatedComponent(pCollisionComponent);
	pProjectileMovementComponent->InitialSpeed = 0.0f;
	pProjectileMovementComponent->MaxSpeed = 0.0f;
	pProjectileMovementComponent->bRotationFollowsVelocity = true;
	pProjectileMovementComponent->bShouldBounce = false;
	pProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	// Lock movement along the Y-axis
	pProjectileMovementComponent->bConstrainToPlane = true;
	pProjectileMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
}

void ABASE_Projectile::LaunchProjectile(const FVector& aLaunchDirection) {
	pProjectileMovementComponent->Velocity = aLaunchDirection * pProjectileMovementComponent->InitialSpeed;
}

// Called when the game starts or when spawned
void ABASE_Projectile::BeginPlay() {
	Super::BeginPlay();	
}

// Called every frame
void ABASE_Projectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
