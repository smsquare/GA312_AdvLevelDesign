// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BASE_Projectile.generated.h"

UCLASS()
class LEVELCONCEPT_API ABASE_Projectile : public AActor  {
	GENERATED_BODY()

// BASE VARIABLES //
public:
	UPROPERTY(VisibleDefaultsOnly, Category="Projectile")
	USphereComponent* pCollisionComponent;
	UPROPERTY(VisibleAnywhere, Category="Movement")
	UProjectileMovementComponent* pProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category="Info")
	FName ProjectileName;

// BASE METHODS //
public:	
	ABASE_Projectile(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
