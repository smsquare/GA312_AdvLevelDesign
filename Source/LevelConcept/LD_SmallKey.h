// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LD_SmallKey.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_SmallKey : public AActor  {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "SmallKey")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, Category = "Mesh|SmallKey")
	UStaticMeshComponent* SmallKeyMesh;

public:	
	ALD_SmallKey();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Category = "Proximity")
	void PickupDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};