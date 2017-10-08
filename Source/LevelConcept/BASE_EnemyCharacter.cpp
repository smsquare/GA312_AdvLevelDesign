// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "BASE_EnemyCharacter.h"

/****************************
		FEnemyStats
****************************/
FEnemyStats::FEnemyStats() {
	HPMax = 0.0f;
	HPCurr = 0.0f;
	MoveSpeed = 0.0f;
	IsDead = false;
}

FEnemyStats::FEnemyStats(float currHP, float maxHP, float movespeed) :
	HPCurr(currHP), HPMax(maxHP), MoveSpeed(movespeed), IsDead(false) {
}

float FEnemyStats::GetHPCurr() const {
	return HPCurr;
}

bool FEnemyStats::GetIsDead() const {
	return IsDead;
}

/*
FOR INITIALIZATION ONLY
All other instances of setting HPCurr need to be through RecieveDamage()
*/
void FEnemyStats::SetHPCurr(const float& amount) {
	HPCurr = amount;
}
void FEnemyStats::RecieveDamage(float amount) {
	HPCurr -= amount;
	if (HPCurr <= 0.0f && !IsDead) {
		OnDeath();
	}
}

void FEnemyStats::OnDeath() {
	IsDead = true;
}

/***************************** 
	ABASE_EnemyCharacter 
****************************/
ABASE_EnemyCharacter::ABASE_EnemyCharacter() {
	PrimaryActorTick.bCanEverTick = true;
	EnemyStats.HPMax = 100;
	EnemyStats.MoveSpeed = 100;
}

float ABASE_EnemyCharacter::GetHPCurr() const {
	return EnemyStats.GetHPCurr();
}
void ABASE_EnemyCharacter::EnemyRecieveDamage(float amount) {
	EnemyStats.RecieveDamage(amount);
}
void ABASE_EnemyCharacter::BeginPlay() {
	Super::BeginPlay();	
	EnemyStats.SetHPCurr(75);
}

void ABASE_EnemyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

bool ABASE_EnemyCharacter::GetIsDead() const {
	return EnemyStats.GetIsDead();
}

// Called to bind functionality to input
//void ABASE_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

