// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "BASE_Projectile.h"
#include "LD_Wall.h"
#include "LD_Player.h"

// FWallInformation( InitSlideVelocity, SlideAcceration, WallHoldDuration)
const FWallInformation ALD_Wall::RoughWallInfo = FWallInformation(1.0f, 75.0f, 1.5f);
const FWallInformation ALD_Wall::SlipperyWallInfo = FWallInformation(100.0f, 1300.0f, 0.05f);
const FWallInformation ALD_Wall::SmoothWallInfo = FWallInformation(3.0f, 198.0f, 0.85f);
const FWallInformation ALD_Wall::StickyWallInfo = FWallInformation(1.0f, 25.0f, 2.0f);

// Sets default values
ALD_Wall::ALD_Wall() {
	PrimaryActorTick.bCanEverTick = false;
	
	WallType = EWallFrictionType::WFT_Rough;
	// Secret Wall variables //
	IsSecretWall = false;
	SecretDamageType = ESecretWallDamageType::SD_INVALID;
	HasSecretMessage = false;
	SecretMessage = "I'M A DEFAULT SECRET MESSAGE :D";

	// Create mesh and set it as root.
	WallMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall Mesh"));
	WallMesh->SetCollisionProfileName(FName("NoCollision"));
	RootComponent = WallMesh;
	// Create collider
	WallCollider = CreateDefaultSubobject<UBoxComponent>("Wall Collider");
	WallCollider->SetCollisionProfileName(FName("CustomWall"));	
	WallCollider->AttachToComponent(WallMesh, FAttachmentTransformRules::KeepRelativeTransform);
	//Create FallOffPoint
	FallOffPoint = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Fall Off Point"));
	FallOffPoint->SetCollisionProfileName(FName("FallOffPoint"));
	FallOffPoint->AttachToComponent(WallMesh, FAttachmentTransformRules::KeepWorldTransform);
	FallOffPoint->OnComponentBeginOverlap.AddDynamic(this, &ALD_Wall::FallOffDetection);
	FallOffPoint->OnComponentEndOverlap.AddDynamic(this, &ALD_Wall::FallOffEndOverlap);
}

// Called when the game starts or when spawned
void ALD_Wall::BeginPlay() {
	Super::BeginPlay();	
	if (IsSecretWall && SecretDamageType == ESecretWallDamageType::SD_INVALID) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(
			-1, 10.0f, FColor::Red, "ERROR: " + GetActorLabel() + ": MUST SET SecretDamageType!");
	}
}

FWallInformation ALD_Wall::GetWallInfo() {
	FWallInformation wallInfo;
	switch (this->WallType) {
	case EWallFrictionType::WFT_Rough:
		wallInfo = RoughWallInfo;
		break;
	case EWallFrictionType::WFT_Slippery:
		wallInfo = SlipperyWallInfo;
		break;
	case EWallFrictionType::WFT_Smooth:
		wallInfo = SmoothWallInfo;
		break;
	case EWallFrictionType::WFT_Sticky:
		wallInfo = StickyWallInfo;
		break;
	default:
		wallInfo = RoughWallInfo;
		break;
	}
	return wallInfo;
}

ESecretWallDamageType ALD_Wall::GetSecretDamageType() const {
	return SecretDamageType;
}

void ALD_Wall::SecretWallHit() {
	if (IsSecretWall) {
		ShowSecretWallUI();
		WallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WallCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WallMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WallMesh->SetVisibility(false);
	}
}

bool ALD_Wall::CanProjectileDamageWall(EProjectileOwner projectileSource) {
	bool result = false;

	switch (projectileSource) {
	case EProjectileOwner::PO_Player:
		if (SecretDamageType == ESecretWallDamageType::SD_PLAYERPROJECTILE ||
			SecretDamageType == ESecretWallDamageType::SD_BOTHPROJECTILE ||
			SecretDamageType == ESecretWallDamageType::SD_ANY) {
			result = true;
		}
		break;
	case EProjectileOwner::PO_Enemey:
		if (SecretDamageType == ESecretWallDamageType::SD_ENEMYPROJECTILE ||
			SecretDamageType == ESecretWallDamageType::SD_BOTHPROJECTILE || 
			SecretDamageType == ESecretWallDamageType::SD_ANY) {
			result = true;
		}
		break;
	default:
		result = false;
		break;
	}

	return result;
}

void ALD_Wall::SecretWallShot(EProjectileOwner projectileSource) {
	if (CanProjectileDamageWall(projectileSource)) {
		SecretWallHit();
	}
}

void ALD_Wall::FallOffDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) )
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "FallOffDetection: HIT");
		ALD_Player* player = Cast<ALD_Player>(OtherActor);
		if (player) {
			if (player->JumpStats.GetHangingOnWall() == true) {
				player->FallOffWall();
			}
			else {				
				player->JumpStats.SetFallOffPointTouched(true);
			}
		}
	}
}

void ALD_Wall::FallOffEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if ((OtherActor != nullptr) && (OtherActor != this)) {
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "STOPPED TOUCHING THE FALLOFF POINT");
		
		ALD_Player* player = Cast<ALD_Player>(OtherActor);
		if (player) {
			player->JumpStats.SetFallOffPointTouched(false);
		}
	}
}

//void ALD_Wall::SecretWallHitDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
//
//}

FString ALD_Wall::GetEnumValueToString(const FString& EnumName, EWallFrictionType EnumValue) {
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!enumPtr) {
		return FString("Invalid");
	}
	return enumPtr->GetEnumName((int32)EnumValue);
}