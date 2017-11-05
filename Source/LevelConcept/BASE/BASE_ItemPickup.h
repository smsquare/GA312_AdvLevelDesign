// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BASE_ItemPickup.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8 {
	PT_INVALID = 0	UMETA(Hidden, DisplayName = "INVALID"),
	PT_CONSUMABLE	UMETA(DisplayName = "Consumable"),
	PT_WEAPON		UMETA(DisplayName = "Weapon"),
	PT_UPGRADE		UMETA(DisplayName = "Upgrade"),
	MAX				UMETA(Hidden)
};

UCLASS()
class LEVELCONCEPT_API ABASE_ItemPickup : public AActor {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	EPickupType PickupType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup|Info")
	FString PickupName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup|Info")
	float PickupRadius;

	UPROPERTY(EditAnywhere, Category = "Mesh|Pickup")
	UStaticMeshComponent* PickupMesh;

	USphereComponent* PickupCollider;

protected:
	bool HasBeenPickedUp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ABASE_ItemPickup();
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	UFUNCTION(Category = "Proximity")
	virtual void PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};