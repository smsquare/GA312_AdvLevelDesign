// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LD_SmallKey.h"
#include "LD_Door.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_Door : public AActor {
	GENERATED_BODY()
public:
	// VARIABLES //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool IsOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (EditCondition = "!IsOpen"))
	bool IsLocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (EditCondition = "IsLocked"))
	EKeyColor KeyNeeded;

	// MESHES //
	UPROPERTY(EditAnywhere, Category = "Mesh|DoorFrame")
	UStaticMeshComponent* DoorFrameMesh;
	UPROPERTY(EditAnywhere, Category = "Mesh|Door")
	UStaticMeshComponent* DoorMesh;

	// COLLIDER //
	UPROPERTY(EditAnywhere, Category = "Collider")
	UBoxComponent* DoorCollider;

public:	
	// Sets default values for this actor's properties
	ALD_Door(const FObjectInitializer& ObjectInitializer);
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Door")
	bool GetIsLocked() const;

	UFUNCTION(BlueprintCallable, Category = "Door")
	bool OpenDoor(uint8 numKeys);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
