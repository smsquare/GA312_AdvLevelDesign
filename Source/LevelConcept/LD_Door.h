// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LD_Door.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_Door : public AActor {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool IsOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Lock", meta = (EditCondition = "!IsOpen"))
	bool IsLocked;
	
	UPROPERTY(EditAnywhere, Category = "Mesh|DoorFrame")
	UStaticMeshComponent* DoorFrameMesh;
	UPROPERTY(EditAnywhere, Category = "Mesh|Doors")
	UStaticMeshComponent* LeftDoorMesh;
	UPROPERTY(EditAnywhere, Category = "Mesh|Doors")
	UStaticMeshComponent* RightDoorMesh;
	UPROPERTY(EditAnywhere, Category = "Mesh|Doors")
	UStaticMeshComponent* SmallLockMesh;
	UPROPERTY(EditAnywhere, Category = "Collider")
	UBoxComponent* DoorCollider;

public:	
	// Sets default values for this actor's properties
	ALD_Door(const FObjectInitializer& ObjectInitializer);
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Door")
	bool GetIsLocked() const;
	
	class ALD_Player;

	UFUNCTION(BlueprintCallable, Category = "Door")
	bool OpenDoor(uint8 numKeys);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
