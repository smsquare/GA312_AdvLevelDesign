// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BASE_EnemyCharacter.generated.h"

USTRUCT()
struct LEVELCONCEPT_API FEnemyStats {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString Name;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	//TODO: Add enemy type

	// Base Stats //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|HP")
	float HPMax;
	// Movement //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float MoveSpeed;

private:
	float HPCurr;
	bool IsDead;

public:
	// Constructors //
	FEnemyStats();
	FEnemyStats(float currHP, float maxHP, float movespeed);

	// General Methods //
	void RecieveDamage(float amount);

	// GETTERS //
	FORCEINLINE float GetHPCurr() const;
	FORCEINLINE bool GetIsDead() const;
	// SETTERS //
	void SetHPCurr(const float& amount);
private:
	void OnDeath();
};
UCLASS()
class LEVELCONCEPT_API ABASE_EnemyCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FEnemyStats EnemyStats;
public:
	// Sets default values for this character's properties
	ABASE_EnemyCharacter();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	FORCEINLINE float GetHPCurr() const;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void EnemyRecieveDamage(float amount);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	FORCEINLINE bool GetIsDead() const; 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
