// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "BASE_ItemPickup.h"
#include "LD_DamageUpgradePickup.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_DamageUpgradePickup : public ABASE_ItemPickup {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|HealthPack")
	float IncreaseDamageAmt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|HealthPack")
	float RotationSpeed;
	
public:
	ALD_DamageUpgradePickup();
	void Tick(float DeltaTime) override;
	UFUNCTION(Category = "Proximity")
	void PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item|Particles")
	void ActivateDamageUpgradeParticle();
};
