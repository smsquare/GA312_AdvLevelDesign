// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "LD_Wall.h"
#include "LD_Player.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementType : uint8 {
	CMT_WallSlide = 0 UMETA(DisplayName = "Wall Slide")
};

USTRUCT() 
struct LEVELCONCEPT_API FDetectWallHitInfo {
	GENERATED_USTRUCT_BODY()

private:
	bool CanJump;
	// Wall Direction relative to player
	// -1: Left
	//  1: Right
	//  0: NULL
	int8 WallDirection;
	// WallHit information
	float WallHitSlideVelocity;
	float WallHitSlideAcceleration;
	float WallHitHoldDuration;

public:
	FDetectWallHitInfo() {
		CanJump = false;
		WallDirection = 0;
		WallHitSlideVelocity = 0.0f;
		WallHitSlideAcceleration = 0.0f;
		WallHitHoldDuration = 0.0f;
	}
	FORCEINLINE float GetWallHitSlideVelocity() const {
		return WallHitSlideVelocity;
	}
	FORCEINLINE float GetWallHitSlideAcceleration() const {
		return WallHitSlideAcceleration;
	}
	FORCEINLINE float GetWallHitHoldDuration() const {
		return WallHitHoldDuration;
	}
	FORCEINLINE bool GetCanJump() const {
		return CanJump;
	}
	FORCEINLINE int8 GetWallDirection() const {
		return WallDirection;
	}

	void SetCanJump(const bool& value) {
		CanJump = value;
	}
	void SetWallDirection(const int8& value) {
		(value < 2 && value > -2) ? (WallDirection = value) : (WallDirection = 0);
	}

	void SetWallHitInfo(const FWallInformation& value) {
		WallHitSlideVelocity = value.InitSlideVelocity;
		WallHitSlideAcceleration = value.SlideAcceration;
		WallHitHoldDuration = value.WallHoldDuration;
	}

};

USTRUCT()
struct LEVELCONCEPT_API FJumpVariables {
	GENERATED_USTRUCT_BODY()

public:
	// Display debug info
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool DebugInfo;
	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpPower;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float MaxSlideVelocity;
	
	UPROPERTY(EditAnywhere, Category = "Jump")
	float FallOffWallHeight;

	/****** CONSTRUCTOR ******/
	FJumpVariables() {
		DebugInfo = false;
		JumpPower = 1000.0f;
		MaxSlideVelocity = 1000.0f;
		FallOffWallHeight = 0.0;

		SlideAcceleration = 98.0f;
		CurrentSlideVelocity = 0.0f;
		WallOnPlayerSide = 0;
		WallHoldDuration = 0.0f;

		FallOffPointTouched = false;
		HangingOnWall = false;
		CanDoubleJump = false;
		DoubleJumpEnabled = false;
		
	}

	/****** Methods for FPlayerJumpVariables ******/
	void DisplayWallSlideDebugInfo() {
		if (DebugInfo) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Blue, TEXT("CurrSlideVelocity: " + FString::SanitizeFloat(CurrentSlideVelocity)));
		}
	}
	
	void ApplyWallSlideAcceleration(const float& deltaTime) {
		CurrentSlideVelocity += (SlideAcceleration * deltaTime);
		if (CurrentSlideVelocity > MaxSlideVelocity) {
			CurrentSlideVelocity = MaxSlideVelocity;
		}
	}

	void ResetWallSlideVariable() {
		CurrentSlideVelocity = 0.0f;
	}

	/****** Setters for private variables ******/
	void SetHangingOnWall(const bool& value) {
		HangingOnWall = value;
	}

	void SetFallOffPointTouched(const bool& value) {
		FallOffPointTouched = value;
	}

	void SetWallOnPlayerSide(const int8& value) {
		(value < 2 && value > -2) ? (WallOnPlayerSide = value) : (WallOnPlayerSide = 0);
	}

	// If DoubleJump is disabled, set to false, otherwise, it's the value.
	void SetCanDoubleJump(const bool& value) {
		(DoubleJumpEnabled) ? (CanDoubleJump = value) : (CanDoubleJump = false);
	}

	void SetCurrentSlideVelocity(const float& value) {
		(value < MaxSlideVelocity) ? (CurrentSlideVelocity = value) : (CurrentSlideVelocity = MaxSlideVelocity);
	}

	void SetSlideAcceleration(const float& value) {
	//TODO: Maybe consider forcing it to always be greater than 0
		SlideAcceleration = value;
	}
	void SetWallHoldDuration(const float& value) {
		WallHoldDuration = value;
	}
	/****** Getters for private variables ******/
	FORCEINLINE float GetSlideAcceleration() const {
		return SlideAcceleration;
	}

	FORCEINLINE float GetCurrentSlideVelocity() const {
		return CurrentSlideVelocity;
	}

	FORCEINLINE int8 GetWallOnPlayerSide() const {
		return WallOnPlayerSide;
	}

	FORCEINLINE bool GetHangingOnWall() const {
		return HangingOnWall;
	}

	FORCEINLINE bool GetFallOffPointTouched() const {
		return FallOffPointTouched;
	}
	
	FORCEINLINE bool GetCanDoubleJump() const {
		return CanDoubleJump;
	}

	FORCEINLINE bool GetDoubleJumpEnabled() const {
		return DoubleJumpEnabled;
	}

	FORCEINLINE float GetWallHoldDuration() const {
		return WallHoldDuration;
	}
	
	void EnableDoubleJump() {
		DoubleJumpEnabled = true;
		CanDoubleJump = true;
	}
	void DisableDoubleJump() {
		DoubleJumpEnabled = false;
		CanDoubleJump = false;
	}

private:
	float SlideAcceleration;
	float CurrentSlideVelocity;

	// Wall direction relative to player
	// -1: Left
	//  1: Right
	//  0: NULL
	int8 WallOnPlayerSide;

	bool HangingOnWall;
	bool FallOffPointTouched;
	// Can the player double jump; can only be positive if DoubleJumpEnabled is true
	bool CanDoubleJump;

	// Determines if Double Jumping turned on in the game. When false, CanDoubleJump will ALWAYS be false
	bool DoubleJumpEnabled;
	float WallHoldDuration;
};

UENUM(BlueprintType)
enum class EBasicAttackType : uint8 {
	INVALID = 0	UMETA(Hidden),
	LIGHT		UMETA(DisplayName = "Light Attack"),
	HEAVY		UMETA(DisplayName = "Heavy Attack"),
	NONE = 99	UMETA(DisplayName = "None")
};

UCLASS()
class LEVELCONCEPT_API ALD_Player : public ACharacter
{
	GENERATED_BODY()
/********************* PUBLIC VARIABLES *********************/
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stats")
	float HealthCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stats")
	float HealthMax;

	//----------------------- MOVEMENT -------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
	float WalkSpeed;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
	float RunSpeed;

	UPROPERTY(EditAnywhere, Category = "Player|Jump")
	FJumpVariables JumpStats;

	//TODO:IMPLEMENT STATUS EFFECTS

	//----------------------- COMBAT -------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float LightBADamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HeavyBADamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Cooldowns")
	float LightBACooldown; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Cooldowns")
	float HeavyBACooldown;

	//----------------------- INTERACT -------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Levers")
	uint8 NumLeversLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Levers")
	bool NearLever;
	/********************* PUBLIC METHODS *********************/
public:
	
	// Sets default values for this character's properties
	ALD_Player();
	//TODO: AKOA_PROTO_Character(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/****** TICK ******/
	//TODO: IMPLEMENT TICK 
	//virtual void Tick(float DeltaSeconds) override;

	//******************** PLAYER STATS ************************//
	UFUNCTION(BlueprintCallable, Category = "Player|Stats|HP")
	void DamagePlayer(float amount);
	UFUNCTION(BlueprintCallable, Category = "Player|Stats|HP")
	void HealPlayer(float amount);
	UFUNCTION(BlueprintCallable, Category = "Player|Stats|HP")
	void UpgradeMaxHealth(float amount);
	UFUNCTION(BlueprintCallable, Category = "Player|Stats|HP")
	void PickedUpHealthUpgrade();
	UFUNCTION(BlueprintCallable, Category = "Player|Stats|HP")
	bool DidHealthUpgrade() const ;
	UFUNCTION(BlueprintCallable, Category = "Player|Stats|HP")
	void ResetIsHealthUpgraded();
	//********************** MOVEMENT **************************//
	void SetMoveSpeedToRun();
	void SetMoveSpeedToWalk();
	void MoveRight(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE bool GetIsMovementInputDisabled() const;
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetIsMovementInputDisabled(bool isDisabled);
	//********************** INTERACT *************************//
	void PushLever();
	//********************** INVENTORY *************************//
	UFUNCTION(BlueprintCallable, Category = "Inventory|Keys")
	void PickupSmallKey();
	UFUNCTION(BlueprintCallable, Category = "Inventory|Keys")
	uint8 GetNumOfSmallKeys() const;

	//TODO: PickupBossKey();
	//TODO: GetHasBossKey();
	//*********************** INTERACT **************************//
	UFUNCTION(BlueprintCallable, Category = "Interact|Door")
	void OpenDoor();
	//*********************** JUMPING **************************//
	FDetectWallHitInfo DetectWall();
	void PlayerJump();
	UFUNCTION(BlueprintCallable, Category = "Jump")
	FORCEINLINE bool GetUsedDoubleJump() const;
	UFUNCTION(BlueprintCallable, Category = "Jump")
	void SetUsedDoubleJump(const bool& value);
	void PlayerStopJump();
	virtual void Landed(const FHitResult& Hit) override;
	void StartWallSlide();
	void FallOffWall();

	//*********************** COMBAT **************************//
	void PressedLightBasicAttack();
	void PressedHeavyBasicAttack();
	void PressedKick();
	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	EBasicAttackType GetBasicAttackInUse() const;
	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	void ResetBasicAttackInUse();
	void ResetBasicAttackCooldown();
	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	bool GetIsThrowingSomething() const;
	void SetIsThrowingSomething(const bool& value);
	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	bool GetIsPlayerKicking() const;
	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	void ResetIsPlayerKicking();
	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	void UpgradeLBasicDamage(float amount);
	//*********************** TIMERS **************************//
	void StartWallHoldTimer(const float& duration);
	void StartWallSlideTimer(const float& duration);
	void ClearWallHoldTimer();
	void ClearWallSlideTimer();

	void StartBasicAttackCooldownTimer(EBasicAttackType typeofEBAT);

	void StartLightBasicAttackTimer(const float& duration);
	void ClearLightBasicAttackTimer();
	void StartHeavyBasicAttackTimer(const float& duration);
	void ClearHeavyBasicAttackTimer();
	//*********************** WORLD **************************//
	FORCEINLINE const UWorld* GetWorldPtr() const;

/********************* PRIVATE VARIABLES *********************/
private:
	/***** STATS *****/
	bool IsDead;
	bool IsHealthUpgraded;
	/***** MOVEMENT *****/
	bool CanDodge;
	bool IsMovementInputDisabled;
	bool IsRunDisabled;
	bool IsSlidingDownWall;	
	// This is passed to the ANIMBP so it can preform the jump anim again.
	// Put here so I can make the accessors BlueprintCallable
	bool UsedDoubleJump;
	//--------------------- INVENTORY ------------------------//
	uint8 NumOfSmallKeys;
	bool HasBossKey;
	/***** COMBAT *****/
	EBasicAttackType BasicAttackInUse;
	bool IsBasicAttackOnCooldown;
	bool IsThrowingSomething;
	bool IsPlayerKicking;
	/***** TIMERS *****/
	FTimerHandle WallHoldTimer;
	FTimerHandle WallSlideTimer;
	FTimerHandle LightBasicAttackTimer;
	FTimerHandle HeavyBasicAttackTimer;
	/***** WORLD *****/
	const UWorld* WorldPtr;
/********************* PRIVATE METHODS *********************/
private:
	void OnDeath();
};