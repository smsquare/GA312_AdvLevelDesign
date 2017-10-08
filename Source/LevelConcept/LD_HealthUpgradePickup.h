// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "BASE_ItemPickup.h"
#include "LD_HealthUpgradePickup.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_HealthUpgradePickup : public ABASE_ItemPickup {
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|HealthUpgrade")
	float IncreaseHealthAmt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|HealthUpgrade")
	float HealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup|Info")
	float RotationSpeed;
	
public:
	ALD_HealthUpgradePickup();
	void Tick(float DeltaTime) override;
	UFUNCTION(Category = "Proximity")
	void PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Proximity")
	void ActivateHealthUpgradeParticle();
};
