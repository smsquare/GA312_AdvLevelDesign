// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BASE_ItemPickup.h"
#include "LD_HealthPackPickup.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_HealthPackPickup : public ABASE_ItemPickup {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|HealthPack")
	float HealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|HealthPack")
	float RotationSpeed;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* HealParticle;*/

public:
	ALD_HealthPackPickup();
	void Tick(float DeltaTime) override;
	UFUNCTION(Category = "Proximity")
	void PickupDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item|Particles")
	void ActivateHealParticle();
};
