// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "LD_PlayerController.h"
#include "BASE_EnemyCharacter.h"
#include "LD_Door.h"
#include "LD_Lever.h"
#include "BASE_Projectile.h"
#include "LD_Player.h"

// Sets default values
ALD_Player::ALD_Player() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	/***** STATS *****/
	IsDead = false;
	HealthCurrent = 75.0f;
	HealthMax = 100.0f;

	/***** MOVEMENT *****/
	PlayerMovement = EPlayerMovement::PM_DEFAULT;
	IsMovementInputDisabled = false;
	IsRunDisabled = true;

	WalkSpeed = 450.0f;
	RunSpeed = 900.0f;

	IsDashEnabled = false;
	DashDistance = 500.0f;
	DashSpeed = 500.0f;
	DashDamage = 24.0f;
	MostRecentInputDir = 1;
	LocationToDash = 0.0f;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ALD_Player::DashHitEnemy);

	CanDodge = false;
	IsSlidingDownWall = false;
	IsDashing = false;
	IsDashOnCooldown = false;

	/***** INTERACT *****/
	NumLeversLeft = 0;
	NearLever = false;

	/***** JUMPING *****/
	JumpStats.DisableDoubleJump();
	UsedDoubleJump = false;

	// COMBAT //
	IsFireOnCooldown = false;
	LightBADamage = 15.0f;
	HeavyBACooldown = 37.5f;
	BasicAttackInUse = EBasicAttackType::NONE;
	IsBasicAttackOnCooldown = false;
	IsPlayerKicking = false;
	// Get a pointer to the current world
	WorldPtr = GetWorld();
}

// Called when the game starts or when spawned
void ALD_Player::BeginPlay() {
	Super::BeginPlay();	
}

void ALD_Player::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	// Clear ALL timers on this object.
	GetWorldPtr()->GetTimerManager().ClearAllTimersForObject(this);	
}

// Called every frame
void ALD_Player::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	switch (PlayerMovement) {
	case EPlayerMovement::PM_DASH:
		DashMovement(DeltaTime);
		break;
	case EPlayerMovement::PM_WALLSLIDE:
		SlidingWallMovement(DeltaTime);
		break;
	default:
		// Do nothing, it's handled by the CharacterMovementComponent
		break;
	}
}

// Called to bind functionality to input
void ALD_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/***** AXIS BINDING *****/
	InputComponent->BindAxis("MoveRight", this, &ALD_Player::MoveRight);
	/***** ACTION BINDING *****/
	InputComponent->BindAction("Run", IE_Pressed, this, &ALD_Player::SetMoveSpeedToRun);
	InputComponent->BindAction("Run", IE_Released, this, &ALD_Player::SetMoveSpeedToWalk);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ALD_Player::PlayerJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ALD_Player::PlayerStopJump);
	InputComponent->BindAction("Dash", IE_Pressed, this, &ALD_Player::PlayerDash);
	InputComponent->BindAction("DirInputLeft", IE_Pressed, this, &ALD_Player::SetInputDirLeft);
	InputComponent->BindAction("DirInputRight", IE_Pressed, this, &ALD_Player::SetInputDirRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ALD_Player::Fire);
	InputComponent->BindAction("LightBasicAttack", IE_Pressed, this, &ALD_Player::PressedLightBasicAttack);
	InputComponent->BindAction("HeavyBasicAttack", IE_Pressed, this, &ALD_Player::PressedHeavyBasicAttack);
	InputComponent->BindAction("Kick", IE_Pressed, this, &ALD_Player::PressedKick);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ALD_Player::OpenDoor);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ALD_Player::PushLever);
	InputComponent->BindAction("DEBUG_ToggleDoubleJump", IE_Pressed, this, &ALD_Player::DEBUG_ToggleDoubleJump);
	InputComponent->BindAction("DEBUG_ToggleDash", IE_Pressed, this, &ALD_Player::DEBUG_ToggleDash);
}

void ALD_Player::DamagePlayer(float amount) {
	HealthCurrent -= amount;
	if (HealthCurrent <= 0.0 && !IsDead) {
		OnDeath();
	}
}

void ALD_Player::HealPlayer(float amount) {
	HealthCurrent += amount;
	if (HealthCurrent > HealthMax) {
		HealthCurrent = HealthMax;
	}
}

void ALD_Player::UpgradeMaxHealth(float amount) {
	HealthMax += amount;
}

void ALD_Player::PickedUpHealthUpgrade() {
	IsHealthUpgraded = true;
}

bool ALD_Player::DidHealthUpgrade() const {
	return IsHealthUpgraded;
}

void ALD_Player::ResetIsHealthUpgraded() {
	IsHealthUpgraded = false;
}

//////////////////////////////////////////////////////////////
// 						   MOVEMENT 						//
//				Methods to handle movement.					//
//				Walking, Running, Jumping, Sliding			//
//////////////////////////////////////////////////////////////
void ALD_Player::SetMoveSpeedToRun() {
	if (IsRunDisabled == false) {
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void ALD_Player::SetMoveSpeedToWalk() {
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ALD_Player::MoveRight(float Amount) {
	// Move right if Amount > 0; left if Amount is < 0
	// Only move if the controller is set up, Amount is not 0, MovementInput not disabled
	if (Controller && Amount && GetIsMovementInputDisabled() == false) {
		AddMovementInput(FVector(1.0, 0.0, 0.0), Amount);
	}
}

void ALD_Player::PlayerDash() {
	if (IsDashEnabled && !IsDashOnCooldown) {
		PlayerMovement = EPlayerMovement::PM_DASH;
		GetCharacterMovement()->SetMovementMode(MOVE_Custom, (uint8)ECustomMovementType::CMT_Dash);
		IsDashing = true;
		IsDashOnCooldown = true;
		SetIsMovementInputDisabled(true);
		LocationToDash = GetActorLocation().X + (DashDistance * MostRecentInputDir);
		GetCapsuleComponent()->SetCollisionProfileName(FName("Dashing"));
	}
}

void ALD_Player::DashMovement(float deltaTime) {

	if (IsDashing) {
		PlayerMovement = EPlayerMovement::PM_DASH;

		FVector location = GetActorLocation();
		float distance = MostRecentInputDir * DashSpeed * deltaTime;
		location.X = location.X + distance;

		// If dashing to the left
		if (MostRecentInputDir < 0) {
			if (location.X < LocationToDash) {
				location.X = LocationToDash;
				ResetDash();
			}
		}
		else {
			if (location.X > LocationToDash) {
				location.X = LocationToDash;
				ResetDash();
			}
		}

		FHitResult outSweepHitResult;
		bool successfulMove = SetActorLocation(location, true, &outSweepHitResult);
		if (!successfulMove) {
			ResetDash();
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "SetActorLocation sweeping failed while dashing!");
		}
	}
}

void ALD_Player::DashHitEnemy(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	ABASE_EnemyCharacter* enemy = Cast<ABASE_EnemyCharacter>(OtherActor);
	if (enemy) {
		enemy->EnemyRecieveDamage(DashDamage);
	}
}
bool ALD_Player::GetIsDashing() const {
	return IsDashing;
}

void ALD_Player::ResetDash() {
	PlayerMovement = EPlayerMovement::PM_DEFAULT;
	IsDashing = false;
	IsDashOnCooldown = false;
	SetIsMovementInputDisabled(false);
	GetCapsuleComponent()->SetCollisionProfileName(FName("Player"));
	if (GetCharacterMovement()->IsFalling()) {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	} else {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}	
}

void ALD_Player::DEBUG_ToggleDash() {
	(IsDashEnabled == true) ? IsDashEnabled = false : IsDashEnabled = true;
}


void ALD_Player::SetInputDirLeft() {
	if (GetIsMovementInputDisabled() == false) {
		MostRecentInputDir = -1;
	}
}

void ALD_Player::SetInputDirRight() {
	if (GetIsMovementInputDisabled() == false) {
		MostRecentInputDir = 1;
	}
}

bool ALD_Player::GetIsMovementInputDisabled() const {
	return IsMovementInputDisabled;
}

void ALD_Player::SetIsMovementInputDisabled(bool isDisabled) {
	IsMovementInputDisabled = isDisabled;
}

void ALD_Player::PushLever() {
	if (NearLever) {
		// Start at player feet
		FVector PlayerLocation = GetActorLocation();
		FVector StartLocation = PlayerLocation + (-35 * GetActorUpVector());
		FVector EndLocation = StartLocation + (25 * GetActorForwardVector());
		float Radius = 25;
		// Array of object types
		// I BELIEVE ObjectTypeQuery1 is WorldStatic and ObjectTypeQuery2 is WorldDynamic
		TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
		// Actors to ignore
		TArray<AActor*>ActorsToIgnore;
		// Hit result
		FHitResult HitResult;

		// Perform the sphere trace 
		bool HitLever = UKismetSystemLibrary::SphereTraceSingleForObjects(
			this, StartLocation, EndLocation, Radius, ObjectTypes,
			false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
		// If you hit something
		if (HitLever) {
			ALD_Lever* lever = Cast<ALD_Lever>(HitResult.GetActor());
			if (lever) {
				if (!lever->HasBeenPulled) {
					lever->PushLever();
				}
				
			}
		}
	}
}

//void ALD_Player::PickupSmallKey() {
//	++NumOfSmallKeys;
//}
//
//uint8 ALD_Player::GetNumOfSmallKeys() const {
//	return NumOfSmallKeys;
//}

void ALD_Player::OpenDoor() {
	// Start at player feet
	FVector PlayerLocation = GetActorLocation();
	FVector StartLocation = PlayerLocation + (-35 * GetActorUpVector());
	FVector EndLocation = StartLocation + (25 * GetActorForwardVector());
	float Radius = 25;
	// Array of object types
	// I BELIEVE ObjectTypeQuery1 is WorldStatic and ObjectTypeQuery2 is WorldDynamic
	TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	// This is the custom wall object type
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery11);
	// Actors to ignore
	TArray<AActor*>ActorsToIgnore;
	// Hit result
	FHitResult HitResult;

	// Perform the sphere trace 
	bool HitDoor = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this, StartLocation, EndLocation, Radius, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	// If you hit something
	if (HitDoor) {
		ALD_Door* door = Cast<ALD_Door>(HitResult.GetActor());
		if (door) {
			
			switch (door->KeyNeeded) {
			case EKeyColor::KC_CYAN:
				if (KeyRing.GetNumOfCyanKeys() > 0) {
					KeyRing.UseKey(EKeyColor::KC_CYAN);
					door->OpenDoor();					
				}
				break;
			case EKeyColor::KC_PURPLE:
				if (GEngine) GEngine->AddOnScreenDebugMessage(
					-1, 2, FColor::Purple, "Player kicked PURPLE Door");
				if (KeyRing.GetNumOfPurpleKeys() > 0) {
					KeyRing.UseKey(EKeyColor::KC_PURPLE);
					door->OpenDoor();
				}
				break;
			case EKeyColor::KC_YELLOW:
				if (KeyRing.GetNumOfYellowKeys() > 0) {
					KeyRing.UseKey(EKeyColor::KC_YELLOW);
					door->OpenDoor();
				}
				break;

			}
		}
	}
}

//////////////////////////////////////////////////////////////
// 						   Inventory						//
//			Methods to handle the player inventory.			//
//////////////////////////////////////////////////////////////
uint8 ALD_Player::GetNumOfKeyByColor(EKeyColor keyColor) const {
	uint8 result = 0;
	switch (keyColor) {
	case EKeyColor::KC_CYAN:
		result = KeyRing.GetNumOfCyanKeys();
		break;
	case EKeyColor::KC_PURPLE:
		result = KeyRing.GetNumOfPurpleKeys();
		break;
	case EKeyColor::KC_YELLOW:
		result = KeyRing.GetNumOfYellowKeys();
		break;
	}
	return result;
}

//////////////////////////////////////////////////////////////
// 						   JUMPING 							//
//				Methods to handle jumping logic.			//
//////////////////////////////////////////////////////////////
FDetectWallHitInfo ALD_Player::DetectWall() {
	// Create HitInfo
	FDetectWallHitInfo DWHitInfo;

	// Create a local variable to keep track of displaying debug info
	EDrawDebugTrace::Type DebugDuration;
	(JumpStats.DebugInfo) ? (DebugDuration = EDrawDebugTrace::ForDuration) : (DebugDuration = EDrawDebugTrace::None);

	// Create local varialbes to be used in SphereTrace
	// Start at player feet
	FVector PlayerLocation = GetActorLocation();
	FVector StartLocation = PlayerLocation + (-35 * GetActorUpVector()) + (-25 * FVector(1.0, 0.0, 0.0));
	FVector EndLocation = StartLocation + (50 * FVector(1.0, 0.0, 0.0));

	// radius of the sphere to trace
	float Radius = 25;
	// Array of object types
	// I BELIEVE ObjectTypeQuery1 is WorldStatic and ObjectTypeQuery2 is WorldDynamic
	TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	// I THINK this is the custom wall object type
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery11);

	// Actors to ignore
	TArray<AActor*>ActorsToIgnore;
	// Hit result
	FHitResult HitResult;
	
	// Perform the sphere trace 
	bool FootHitSomething = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this, StartLocation, EndLocation, Radius, ObjectTypes,
		false, ActorsToIgnore, DebugDuration, HitResult, true);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FootHitSomething ? ("FootHitSomething==true") : ("FootHitSomething == false"));

	// If you hit something
	if (FootHitSomething) {
		if (JumpStats.GetFallOffPointTouched() == false) {
			ALD_Wall* Wall = Cast<ALD_Wall>(HitResult.GetActor());
			if (Wall) {
				DWHitInfo.SetCanJump(true);
				FVector Forward = GetActorForwardVector();
				FVector Right = FVector(0.0f, 1.0f, 0.0f);

				// Take the dot product with the forward vector and the right world vector
				float Dot = Forward.Y * Right.Y + Forward.Z * Right.Z;

				// If negative, wall is to the left, if positive wall is to the right
				(Dot < 0) ? DWHitInfo.SetWallDirection(-1) : DWHitInfo.SetWallDirection(1);

				DWHitInfo.SetWallHitInfo(Wall->GetWallInfo());
			}
		}
	}

	// If you didn't hit a wall, all the values will be the default values; otherwise return the proper results
	return DWHitInfo;
}

void ALD_Player::SlidingWallMovement(float deltaTime) {
	if (IsSlidingDownWall) {
		FVector playerLocation = GetActorLocation();
		JumpStats.ApplyWallSlideAcceleration(deltaTime);
		JumpStats.DisplayWallSlideDebugInfo();
		FVector finalLocation = playerLocation -
			(JumpStats.GetCurrentSlideVelocity() * GetActorUpVector() * deltaTime);
		SetActorLocation(finalLocation);
	}
}

void ALD_Player::PlayerJump() {
	if (GetIsMovementInputDisabled() == false) {
		// If the player is grounded, this is their first jump
		if (!GetCharacterMovement()->IsFalling()) {
			ACharacter::Jump();
		}

		// The player has jumped / is already in the air
		else {
			// Check to see if they can jump off a wall...
			FDetectWallHitInfo WallHitInfo = DetectWall();

			// If you can jump off a wall do it
			if (WallHitInfo.GetCanJump()) {
				// Set movement mode to allow user to hang on / slide on wall
				GetCharacterMovement()->StopMovementImmediately();
				GetCharacterMovement()->SetMovementMode(MOVE_Custom, (uint8)ECustomMovementType::CMT_WallSlide);
				PlayerMovement = EPlayerMovement::PM_WALLSLIDE;
				JumpStats.SetHangingOnWall(true);
				JumpStats.SetWallOnPlayerSide(WallHitInfo.GetWallDirection());
				JumpStats.SetCanDoubleJump(true);
				// Set the stats that you got from the wall hit
				JumpStats.SetCurrentSlideVelocity(WallHitInfo.GetWallHitSlideVelocity());
				JumpStats.SetSlideAcceleration(WallHitInfo.GetWallHitSlideAcceleration());
				JumpStats.SetWallHoldDuration(WallHitInfo.GetWallHitHoldDuration());
				// Set the timer for holding onto the wall
				StartWallHoldTimer(JumpStats.GetWallHoldDuration());
			}
			// Check if you can double jump
			else if (JumpStats.GetCanDoubleJump()) {
				ACharacter::Jump();
				UsedDoubleJump = true;
				JumpStats.SetCanDoubleJump(false);
			}
		}
	}
}

bool ALD_Player::GetUsedDoubleJump() const {
	return UsedDoubleJump;
}

void ALD_Player::SetUsedDoubleJump(const bool& value) {
	UsedDoubleJump = value;
}

// Using the unit circle, 60deg is at ((+/-)1/2, sqrt(3)/2)
// Use this for the launch angle of the player off of the wall
const float ROOT_TWO_OVER_TWO = 0.70710678118;
const float ROOT_THREE_OVER_TWO = 0.86602540378;

void ALD_Player::PlayerStopJump() {
	// If you are hanging on a wall, jump off
	if (JumpStats.GetHangingOnWall()) {
		PlayerMovement = EPlayerMovement::PM_DEFAULT;
		// Multiply JumpStats.GetWallOnPlayerSide into the y value for it to launch in the proper direction
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		GetCharacterMovement()->AddImpulse(
			FVector(
				-1 * JumpStats.GetWallOnPlayerSide() * JumpStats.JumpPower, // -1 is for making sure you apply jump in the proper direction, inverts GetWallOnPlayerSide()
				0.0f,
				JumpStats.JumpPower * ROOT_THREE_OVER_TWO
			),
			true 
		);
		
		JumpStats.SetFallOffPointTouched(false);
		JumpStats.SetHangingOnWall(false);
		IsSlidingDownWall = false;
		ClearWallHoldTimer();
		ClearWallSlideTimer();
	}

	ACharacter::StopJumping();
}

void ALD_Player::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);
	IsSlidingDownWall = false;
	JumpStats.SetFallOffPointTouched(false);
	JumpStats.SetHangingOnWall(false);
	JumpStats.SetWallOnPlayerSide(0);
	//If you can double jump, reset the double jump variable
	if (JumpStats.GetDoubleJumpEnabled()) {
		JumpStats.SetCanDoubleJump(true);
	}
	ClearWallSlideTimer();
}

void ALD_Player::StartWallSlide() {
	ClearWallHoldTimer();
	IsSlidingDownWall = true;
	StartWallSlideTimer(2.0f);
}

void ALD_Player::FallOffWall() {
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	JumpStats.SetFallOffPointTouched(false);
	JumpStats.SetHangingOnWall(false);
	IsSlidingDownWall = false;
	ClearWallSlideTimer();
}

void ALD_Player::DEBUG_ToggleDoubleJump() {
	JumpStats.GetDoubleJumpEnabled() ? 
		JumpStats.DisableDoubleJump() :
		JumpStats.EnableDoubleJump();
}

/*************************************************************************
								COMBAT
**************************************************************************/
void ALD_Player::Fire() {
	
	UWorld* world = GetWorld();
	if (world) {
		ALD_PlayerController* playerController = 
			(ALD_PlayerController*)UGameplayStatics::GetPlayerController(world, 0);
		if (playerController) {

			PlayerWeapon.FireWeapon(world, playerController, this);

			/*
			FVector fireDirection = playerController->GetPlayerAimingDirection();
			fireDirection.Normalize();
			FActorSpawnParameters spawnParameters;
			spawnParameters.Owner = this;
			spawnParameters.Instigator = Instigator;
			spawnParameters.SpawnCollisionHandlingOverride = 
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
			// Spawn projectile
			ABASE_Projectile* projectile = world->SpawnActor<ABASE_Projectile>(
				(UClass*)PlayerWeapon.CurrentProjectile,
				this->GetActorLocation() + playerController->playerGunLocation, 
				fireDirection.ToOrientationRotator(), 
				spawnParameters
			);
			// shoot the projectile
			if (projectile) {
				projectile->LaunchProjectile(fireDirection);
			}
			*/
		}
	}
}

void ALD_Player::PressedLightBasicAttack() {
	if (!IsBasicAttackOnCooldown && !GetCharacterMovement()->IsFalling()) {
		IsBasicAttackOnCooldown = true;
		SetIsMovementInputDisabled(true);
		BasicAttackInUse = EBasicAttackType::LIGHT;
		StartBasicAttackCooldownTimer(EBasicAttackType::LIGHT);
	}
}
void ALD_Player::PressedHeavyBasicAttack() {
	if (!IsBasicAttackOnCooldown && !GetCharacterMovement()->IsFalling()) {
		IsBasicAttackOnCooldown = true;
		SetIsMovementInputDisabled(true);
		BasicAttackInUse = EBasicAttackType::HEAVY;
	}
}
void ALD_Player::PressedKick() {
	// If the player is grounded, kick
	if (!GetCharacterMovement()->IsFalling()) {
		IsPlayerKicking = true;
	}
}

bool ALD_Player::GetIsFireOnCooldown() const {
	return IsFireOnCooldown;
}

void ALD_Player::SetIsFireOnCooldown(const bool& value) {
	IsFireOnCooldown = value;
}

EBasicAttackType ALD_Player::GetBasicAttackInUse() const {
	return BasicAttackInUse;
}

void ALD_Player::ResetBasicAttackInUse() {
	BasicAttackInUse = EBasicAttackType::NONE;
}

void ALD_Player::ResetBasicAttackCooldown() {
	BasicAttackInUse = EBasicAttackType::NONE;
	IsBasicAttackOnCooldown = false;
	IsMovementInputDisabled = false;
}
bool ALD_Player::GetIsThrowingSomething() const {
	return IsThrowingSomething;
}
void ALD_Player::SetIsThrowingSomething(const bool& value) {
	IsThrowingSomething = value;
}
bool ALD_Player::GetIsPlayerKicking() const {
	return IsPlayerKicking;
}
void ALD_Player::ResetIsPlayerKicking() {
	IsPlayerKicking = false;
}

void ALD_Player::UpgradeLBasicDamage(float amount) {
	LightBADamage += amount;
}

/**************************************************************************
	TIMERS - Methods for handling timers.
**************************************************************************/
void ALD_Player::StartWallHoldTimer(const float& duration) {
	// When the timer ends, start sliding down the wall...
	if (GetWorldPtr()) {
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "StartWallHoldTimer " + FString::SanitizeFloat(duration));

		GetWorldPtr()->GetTimerManager().SetTimer(WallHoldTimer, this, &ALD_Player::StartWallSlide, duration, false);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "StartWallHoldTimer");
	}
}

void ALD_Player::StartWallSlideTimer(const float& duration) {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(WallSlideTimer, this, &ALD_Player::FallOffWall, duration, false);
	}
}

void ALD_Player::ClearWallHoldTimer() {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().ClearTimer(WallHoldTimer);
	}
}

void ALD_Player::ClearWallSlideTimer() {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().ClearTimer(WallSlideTimer);
	}
}

void ALD_Player::StartBasicAttackCooldownTimer(EBasicAttackType typeofEBAT) {
	switch (typeofEBAT) {
	case EBasicAttackType::LIGHT: 
		StartLightBasicAttackTimer(LightBACooldown);
		break;
	case EBasicAttackType::HEAVY:
		StartHeavyBasicAttackTimer(HeavyBACooldown);
		break;
	default:
		break;
	}
}

void ALD_Player::StartLightBasicAttackTimer(const float& duration) {
	//LightBasicAttackTimer
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(LightBasicAttackTimer, this, &ALD_Player::ResetBasicAttackCooldown, duration, false);
	}
}

void ALD_Player::ClearLightBasicAttackTimer() {
	GetWorldPtr()->GetTimerManager().ClearTimer(LightBasicAttackTimer);
}

void ALD_Player::StartHeavyBasicAttackTimer(const float& duration) {
	//
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(HeavyBasicAttackTimer, this, &ALD_Player::ResetBasicAttackCooldown, duration, false);
	}
}

void ALD_Player::ClearHeavyBasicAttackTimer() {
	GetWorldPtr()->GetTimerManager().ClearTimer(HeavyBasicAttackTimer);
}

//*********************** WORLD **************************//
const UWorld* ALD_Player::GetWorldPtr() const {
	return WorldPtr;
}

//*********************** ONDEATH **************************//
void ALD_Player::OnDeath() {
	IsDead = true;
}