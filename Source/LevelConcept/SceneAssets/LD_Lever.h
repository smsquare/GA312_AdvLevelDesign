// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LD_Lever.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_Lever : public AActor  {
	GENERATED_BODY()
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lever")
	bool HasBeenPulled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lever")
	UStaticMeshComponent* LeverHandleMesh;
	UStaticMeshComponent* LeverBaseMash;

	UPROPERTY(BlueprintReadWrite, Category = "Collision")
	UBoxComponent* BoxCollider;
public:	
	// Sets default values for this actor's properties
	ALD_Lever();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Lever")
	void PushLever();

	//virtual void LeverDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
