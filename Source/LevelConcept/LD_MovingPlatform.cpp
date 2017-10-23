// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "LD_MovingPlatform.h"

FDebugRequiredInfo::FDebugRequiredInfo() {
	Direction = EPlatformDirection::PO_INVALID;
	HStartDir = EPlatformStartDirH::H_INVALID;
	VStartDir = EPlatformStartDirV::V_INVALID;
	DStartDir = EPlatformStartDirD::D_INVALID;
	DistanceToMove = 0.0f;
}

FDebugRequiredInfo::FDebugRequiredInfo (
	EPlatformDirection dir, EPlatformStartDirH startDirH, EPlatformStartDirV startDirV, EPlatformStartDirD startDirD, float moveDistance, FVector platformLocation) :
	Direction(dir),	HStartDir(startDirH), VStartDir(startDirV), DStartDir(startDirD), DistanceToMove(moveDistance), PlatformLocation(platformLocation) {
}

void FDebugRequiredInfo::operator=(const FDebugRequiredInfo& value) {
	Direction = value.Direction;
	HStartDir = value.HStartDir;
	VStartDir = value.VStartDir;
	DStartDir = value.DStartDir;
	DistanceToMove = value.DistanceToMove;
	PlatformLocation = value.PlatformLocation;
}

FDebugPlatformInfo::FDebugPlatformInfo() {
	IsDebugInfoEnabled = false;
	DebugLineThickness = 5.0f;
	DebugPlatMat = nullptr;
	DebugLineMat = nullptr;
}

FVector FDebugPlatformInfo::GetDebugPlatformMeshLocation(FDebugRequiredInfo reqDebugInfo) {
	// Set position based on platform info
	FVector position = reqDebugInfo.PlatformLocation;
	switch (reqDebugInfo.Direction) {
	case EPlatformDirection::PO_Horizonatal:
		if (reqDebugInfo.HStartDir == EPlatformStartDirH::H_Left) {
			position.X = position.X - reqDebugInfo.DistanceToMove;
		}
		else if (reqDebugInfo.HStartDir == EPlatformStartDirH::H_Right) {
			position.X = position.X + reqDebugInfo.DistanceToMove;
		}
		break;
	case EPlatformDirection::PO_Vertical:
		if (reqDebugInfo.VStartDir == EPlatformStartDirV::V_Up) {
			position.Z = position.Z + reqDebugInfo.DistanceToMove;
		}
		else if (reqDebugInfo.VStartDir == EPlatformStartDirV::V_Down) {
			position.Z = position.Z - reqDebugInfo.DistanceToMove;
		}
		break;
	case EPlatformDirection::PO_Depth:
		if (reqDebugInfo.DStartDir == EPlatformStartDirD::D_In) {
			position.Y = position.Y + reqDebugInfo.DistanceToMove;
		}
		else if (reqDebugInfo.DStartDir == EPlatformStartDirD::D_Out) {
			position.Y = position.Y - reqDebugInfo.DistanceToMove;
		}
	break;

	default:
		break;
	}
	return position;
}

// Get the mid point between A and B
// A < B
float FDebugPlatformInfo::GetMiddlePointBA(float pointA, float pointB) {
	float distance = pointB - pointA;
	float result = pointB - (distance / 2.0f);
	return result;
}

FVector FDebugPlatformInfo::GetDebugLineLocation(FDebugRequiredInfo reqDebugInfo) {
	// Set position based on platform info
	FVector position = reqDebugInfo.PlatformLocation;
	switch (reqDebugInfo.Direction) {
	case EPlatformDirection::PO_Horizonatal:
		if (reqDebugInfo.HStartDir == EPlatformStartDirH::H_Left) {
			float debugPoint = position.X - reqDebugInfo.DistanceToMove;
			position.X = GetMiddlePointBA(debugPoint, position.X);
		}
		else if (reqDebugInfo.HStartDir == EPlatformStartDirH::H_Right) {
			float debugPoint = position.X + reqDebugInfo.DistanceToMove;
			position.X = GetMiddlePointBA(position.X, debugPoint);
		}
		break;
	case EPlatformDirection::PO_Vertical:
		if (reqDebugInfo.VStartDir == EPlatformStartDirV::V_Up) {
			float debugPoint = position.Z + reqDebugInfo.DistanceToMove;
			position.Z = GetMiddlePointBA(debugPoint, position.Z);
		}
		else if (reqDebugInfo.VStartDir == EPlatformStartDirV::V_Down) {
			float debugPoint = position.Z - reqDebugInfo.DistanceToMove;
			position.Z = GetMiddlePointBA(position.Z, debugPoint);
		}
		break;

//TODO: case EPlatformDirection::PO_Depth
	default:
		break;
	}
	return position;
}

void FDebugPlatformInfo::ScaleDebugLine(FDebugRequiredInfo reqDebugInfo) {
	//FVector position = reqDebugInfo.PlatformLocation;
	//float dist = 0.0;
	switch (reqDebugInfo.Direction) {
	case EPlatformDirection::PO_Horizonatal:
		this->DebugLineMesh->SetRelativeScale3D(
			FVector(
				reqDebugInfo.DistanceToMove,
				10.0f,
				this->DebugLineThickness
			)
		);
		break;
	case EPlatformDirection::PO_Vertical:
		this->DebugLineMesh->SetRelativeScale3D(
			FVector(
				this->DebugLineThickness,
				10.0f,
				reqDebugInfo.DistanceToMove
			)
		);
		break;
	default:
		break;
	}
	//DebugInfo.DebugLineMesh->SetRelativeScale3D(FVector(DebugInfo.DebugLineThickness, 10.0f, ));
}

// Sets default values
ALD_MovingPlatform::ALD_MovingPlatform(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	// One time initialization
	struct FConstructorStatics {
		//ConstructorHelpers::FObjectFinderOptional<UStaticMesh> SphereMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> DebugPlatformMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> DebugLineMesh;
		ConstructorHelpers::FObjectFinder<UMaterialInstance> DebugPlatformMat;
		ConstructorHelpers::FObjectFinder<UMaterialInstance> DebugLineMat;
		FConstructorStatics()
			: //SphereMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere")),
			DebugPlatformMesh(TEXT("/Game/Geometry/Meshes/SM_Platform.SM_Platform")),
			DebugLineMesh(TEXT("/Game/Geometry/Meshes/SM_DebugLine.SM_DebugLine")),
			DebugPlatformMat(TEXT("/Game/Geometry/Materials/IMAT_DebugPlatform.IMAT_DebugPlatform")),
			DebugLineMat(TEXT("/Game/Geometry/Materials/IMAT_DebugLine.IMAT_DebugLine")) {
		}
	};
	static FConstructorStatics ConstructorStatics;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set PlatformMesh as the RootComponent
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	RootComponent = PlatformMesh;
	PlatformMesh->SetStaticMesh(ConstructorStatics.DebugPlatformMesh.Get());

	// Init variables
	Direction = EPlatformDirection::PO_INVALID;
	HorizontalStartDirection = EPlatformStartDirH::H_INVALID;
	VerticalStartDirection = EPlatformStartDirV::V_INVALID;
	DepthStartDirection = EPlatformStartDirD::D_INVALID;
	RadialDirection = EPlatformStartDirR::R_INVALID;
	DistanceToMove = 0.0f;
	MovementSpeed = 0.0f;
	IsTriggerRequired = false;
	HasTriggeredStart = false;
	HasInitialHold = false;
	InitialHoldDuration = 0.0f;
	DoesPlatformPause = false;
	PauseDuration = 0.0f;
	IsPlatDirH = false;
	IsPlatDirV = false;
	IsPlatDirD = false;
	IsPlatDirP = false;
	IsPlatDirR = false;
	IsUsingDistance = false;
	IsPausing = false;
	CurrentRadialRotation = 0.0f;
	DirectionToMove = 0;

	// Initialize LocationMarker
	//LocationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Location Marker"));
	//LocationMarker->AttachToComponent(PlatformMesh, FAttachmentTransformRules::KeepWorldTransform);
	//LocationMarker->SetStaticMesh(ConstructorStatics.SphereMesh.Get());
	//LocationMarker->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	//LocationMarker->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	//LocationMarker->SetVisibility(false);
	//LocationMarker->SetHiddenInGame(true);

	// Spikes
	HasSpikes = false;
	SpikeDirection = EPlatformSpikeDir::PSD_NONE;

	FDebugRequiredInfo reqInfo = GetDebugRequiredInfo();
	// Debug Platform Mesh and material
	DebugInfo.DebugPlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Debug Platform Mesh"));
	DebugInfo.DebugPlatformMesh->AttachToComponent(PlatformMesh, FAttachmentTransformRules::KeepWorldTransform);
	DebugInfo.DebugPlatformMesh->SetStaticMesh(ConstructorStatics.DebugPlatformMesh.Get());
	DebugInfo.DebugPlatformMesh->SetWorldLocation(DebugInfo.GetDebugPlatformMeshLocation(reqInfo));
	DebugInfo.DebugPlatformMesh->SetVisibility(true);
	DebugInfo.DebugPlatformMesh->SetHiddenInGame(true);
	DebugInfo.DebugPlatformMesh->CanCharacterStepUpOn = TEnumAsByte<ECanBeCharacterBase>(ECanBeCharacterBase::ECB_No);
	DebugInfo.DebugPlatformMesh->bGenerateOverlapEvents = false;
	DebugInfo.DebugPlatMat = CreateDefaultSubobject<UMaterialInstance>(TEXT("Debug Platform Material"));
	if (ConstructorStatics.DebugPlatformMat.Succeeded()) {
		DebugInfo.DebugPlatMat = ConstructorStatics.DebugPlatformMat.Object;
		DebugInfo.DebugPlatformMesh->SetMaterial(0, DebugInfo.DebugPlatMat);
	}
	DebugInfo.DebugPlatformMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DebugInfo.DebugPlatformMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Debug Line Mesh
	DebugInfo.DebugLineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Debug Line Mesh"));
	DebugInfo.DebugLineMesh->AttachToComponent(PlatformMesh, FAttachmentTransformRules::KeepWorldTransform);
	DebugInfo.DebugLineMesh->SetStaticMesh(ConstructorStatics.DebugLineMesh.Get());
	DebugInfo.DebugLineMesh->SetWorldLocation(DebugInfo.GetDebugLineLocation(reqInfo));
	DebugInfo.ScaleDebugLine(reqInfo);
	DebugInfo.DebugLineMesh->SetVisibility(true);
	DebugInfo.DebugLineMesh->SetHiddenInGame(true);
	DebugInfo.DebugLineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DebugInfo.DebugLineMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DebugInfo.DebugLineMesh->CanCharacterStepUpOn = TEnumAsByte<ECanBeCharacterBase>(ECanBeCharacterBase::ECB_No);
	DebugInfo.DebugLineMesh->bGenerateOverlapEvents = false;
	DebugInfo.DebugLineMat = CreateDefaultSubobject<UMaterialInstance>(TEXT("Debug Line Material"));
	if (ConstructorStatics.DebugLineMat.Succeeded()) {
		DebugInfo.DebugLineMat = ConstructorStatics.DebugLineMat.Object;
		DebugInfo.DebugLineMesh->SetMaterial(0, DebugInfo.DebugLineMat);
	}
}

// Called when the game starts or when spawned
void ALD_MovingPlatform::BeginPlay() {
	Super::BeginPlay();
	if (WorldPtr == nullptr) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Begin play!");
	}
	PlatformStartLocation = GetActorLocation();
	InitDirectionToMove();

	if (HasInitialHold) {
		IsPausing = true;
		StartInitialHoldTimer();
	} 
}

void ALD_MovingPlatform::StartPlatformMovement() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(
		-1, 3.0f, FColor::Red, "Start" + GetActorLabel() + ": platform movement!");
	HasTriggeredStart = true;
}

// Clears up things like timers and variables
void ALD_MovingPlatform::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	GetWorldPtr()->GetTimerManager().ClearTimer(PlatformPauseTimer);
}

void ALD_MovingPlatform::PostInitializeComponents() {
	Super::PostInitializeComponents();

	WorldPtr = GetOuter()->GetWorld();
	if (WorldPtr == nullptr) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "ERROR: WorldPtr failed!");
	}
	//if (!IsTriggerRequired) {
	//	HasTriggeredStart = true;
	//}
	//else if (IsTriggerRequired) {
	//	HasTriggeredStart = false;
	//}
}


// Called every frame
void ALD_MovingPlatform::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (WorldPtr == nullptr) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, "PlatformTick!");
	}
	if (IsTriggerRequired) {
		if (HasTriggeredStart && !IsPausing) {
			MovePlatform(DeltaTime);
		}
	} 
	else if (!IsTriggerRequired && !IsPausing) {
		MovePlatform(DeltaTime);
	}
}

#if WITH_EDITOR
void ALD_MovingPlatform::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	//Get all components
	TArray<UActorComponent*> ownedComponents;
	GetComponents(ownedComponents);

	// Get the name of the property that changed
	FName propertyName = (PropertyChangedEvent.Property != NULL) ? 
		PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_MovingPlatform, Direction)) {
		ResetPlatDirDropdownsExcept(Direction);
		ResetAllIsPlatDirExcept(Direction);
		SetIsUsingDistance(Direction);
	}

	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_MovingPlatform, HorizontalStartDirection)) {
		DebugInfo.DebugPlatformMesh->SetWorldLocation(DebugInfo.GetDebugPlatformMeshLocation(GetDebugRequiredInfo()));
		DebugInfo.DebugLineMesh->SetWorldLocation(DebugInfo.GetDebugLineLocation(GetDebugRequiredInfo()));
		DebugInfo.ScaleDebugLine(GetDebugRequiredInfo());
	}
	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_MovingPlatform, VerticalStartDirection)) {
		DebugInfo.DebugPlatformMesh->SetWorldLocation(DebugInfo.GetDebugPlatformMeshLocation(GetDebugRequiredInfo()));
		DebugInfo.DebugLineMesh->SetWorldLocation(DebugInfo.GetDebugLineLocation(GetDebugRequiredInfo()));
		DebugInfo.ScaleDebugLine(GetDebugRequiredInfo());
	}
	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_MovingPlatform, DepthStartDirection)) {
		//PlatformMesh->SetRelativeRotation()
		DebugInfo.DebugPlatformMesh->SetWorldLocation(DebugInfo.GetDebugPlatformMeshLocation(GetDebugRequiredInfo()));
		DebugInfo.DebugLineMesh->SetWorldLocation(DebugInfo.GetDebugLineLocation(GetDebugRequiredInfo()));
		DebugInfo.ScaleDebugLine(GetDebugRequiredInfo());
	}

	if (propertyName == GET_MEMBER_NAME_CHECKED(ALD_MovingPlatform, DistanceToMove)) {
		DebugInfo.DebugPlatformMesh->SetWorldLocation(DebugInfo.GetDebugPlatformMeshLocation(GetDebugRequiredInfo()));
		DebugInfo.DebugLineMesh->SetWorldLocation(DebugInfo.GetDebugLineLocation(GetDebugRequiredInfo()));
		DebugInfo.ScaleDebugLine(GetDebugRequiredInfo());
	}

	if (propertyName == GET_MEMBER_NAME_CHECKED(FDebugPlatformInfo, IsDebugInfoEnabled)) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, DebugInfo.GetDebugPlatformMeshLocation(GetDebugRequiredInfo()).ToString());
	}

	if (propertyName == GET_MEMBER_NAME_CHECKED(FDebugPlatformInfo, DebugLineThickness)) {
		DebugInfo.ScaleDebugLine(GetDebugRequiredInfo());
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

FDebugRequiredInfo ALD_MovingPlatform::GetDebugRequiredInfo() const {
	FDebugRequiredInfo info = FDebugRequiredInfo(
		Direction, 
		HorizontalStartDirection, 
		VerticalStartDirection, 
		DepthStartDirection,
		DistanceToMove, 
		GetActorLocation()
	);
	return info;
}

void ALD_MovingPlatform::SetIsPausing(bool value) {
	if (DoesPlatformPause && value == true) {
		IsPausing = true;
		StartPlatformPauseTimer();
	}
	else {
		IsPausing = false;
	}
}

void ALD_MovingPlatform::StartInitialHoldTimer() {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(
			PlatformPauseTimer, this, &ALD_MovingPlatform::ResetPlatformPauseTimer, InitialHoldDuration, false
		);
	}
}

void ALD_MovingPlatform::StartPlatformPauseTimer() {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().SetTimer(
			PlatformPauseTimer, this, &ALD_MovingPlatform::ResetPlatformPauseTimer, PauseDuration, false
		);
	}
}

void ALD_MovingPlatform::ResetPlatformPauseTimer() {
	if (GetWorldPtr()) {
		GetWorldPtr()->GetTimerManager().ClearTimer(PlatformPauseTimer);
	}
	SetIsPausing(false);
	
}

const UWorld* ALD_MovingPlatform::GetWorldPtr() const {
	return WorldPtr;
}

void ALD_MovingPlatform::InitDirectionToMove() {
	switch (Direction) {
	case EPlatformDirection::PO_Horizonatal:
		// -1 left | 1 right
		if (HorizontalStartDirection == EPlatformStartDirH::H_Left) {
			DirectionToMove = -1;
		} else if (HorizontalStartDirection == EPlatformStartDirH::H_Right) {
			DirectionToMove = 1;
		} else {
			if (GEngine) GEngine->AddOnScreenDebugMessage(
				-1, 3.0f, FColor::Red, "ERROR " + GetActorLabel() + ": MUST SET HorizontalStartDirection!");
		} 
		break;
	case EPlatformDirection::PO_Vertical:
		if (VerticalStartDirection == EPlatformStartDirV::V_Up) {
			DirectionToMove = 1;
		}
		else if (VerticalStartDirection == EPlatformStartDirV::V_Down) {
			DirectionToMove = -1;
		}
		else {
			if (GEngine) GEngine->AddOnScreenDebugMessage(
				-1, 3.0f, FColor::Red, "ERROR " + GetActorLabel() + ": MUST SET VerticalStartDirection!");
		}
		break;
	// DEPTH //
	case EPlatformDirection::PO_Depth:
		if (DepthStartDirection == EPlatformStartDirD::D_In) {
			DirectionToMove = 1;
		}
		else if (DepthStartDirection == EPlatformStartDirD::D_Out) {
			DirectionToMove = -1;
		}
		else {
			if (GEngine) GEngine->AddOnScreenDebugMessage(
				-1, 3.0f, FColor::Red, "ERROR " + GetActorLabel() + ": MUST SET DepthStartDirection!");
		}
		break;
	case EPlatformDirection::PO_Path:
		break;
	case EPlatformDirection::PO_Radial:
		if (RadialDirection == EPlatformStartDirR::R_Clockwise) {
			DirectionToMove = 1;
		}
		else if (RadialDirection == EPlatformStartDirR::R_CounterClockWise) {
			DirectionToMove = -1;
		}
		else {
			if (GEngine) GEngine->AddOnScreenDebugMessage(
				-1, 3.0f, FColor::Red, "ERROR " + GetActorLabel() + ": MUST SET RadialDirection!");
		}
		break;
	default:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "ERROR " + GetActorLabel() + ": MUST SET DIRECTION!");
		break;
	}
}

bool ALD_MovingPlatform::IsHeadingInStartDirection() {
	bool result = false;
	if (IsPlatDirH) {
		if ( (HorizontalStartDirection == EPlatformStartDirH::H_Left && DirectionToMove == -1) ||
			 (HorizontalStartDirection == EPlatformStartDirH::H_Right && DirectionToMove == 1) ) {
			result = true;
		}
		else {
			result = false;
		}
	}
	else if (IsPlatDirV) {
		if ( (VerticalStartDirection == EPlatformStartDirV::V_Up && DirectionToMove == 1) ||
			 (VerticalStartDirection == EPlatformStartDirV::V_Down && DirectionToMove == -1) ) {
			result = true;
		}
		else {
			result = false;
		}
	}
	// DEPTH //
	else if (IsPlatDirD) {
		if ((DepthStartDirection == EPlatformStartDirD::D_In && DirectionToMove == 1) ||
			(DepthStartDirection == EPlatformStartDirD::D_Out && DirectionToMove == -1)) {
			result = true;
		}
		else {
			result = false;
		}
	}

	return result;
}

float ALD_MovingPlatform::LocationMaxDistance() {
	float max = 0.0f;

	// if started right
	if (HorizontalStartDirection == EPlatformStartDirH::H_Right) {
		//heading right
		if (DirectionToMove == 1) {
			max = PlatformStartLocation.X + DistanceToMove;
		}
		//heading left
		else if (DirectionToMove == -1) {
			max = PlatformStartLocation.X;
		}
	}
	// if started left
	else if (HorizontalStartDirection == EPlatformStartDirH::H_Left) {
		//heading left
		if (DirectionToMove == -1) {
			max = PlatformStartLocation.X - DistanceToMove;
		}
		//heading right
		else if (DirectionToMove == 1) {
			max = PlatformStartLocation.X;
		}
	}
	// if started up 
	else if (VerticalStartDirection == EPlatformStartDirV::V_Up) {
		// heading up
		if (DirectionToMove == 1) {
			max = PlatformStartLocation.Z + DistanceToMove;
		}
		// heading down
		else if (DirectionToMove == -1) {
			max = PlatformStartLocation.Z;
		}
	}
	// if started down 
	else if (VerticalStartDirection == EPlatformStartDirV::V_Down) {
		// heading up
		if (DirectionToMove == 1) {
			max = PlatformStartLocation.Z;
		}
		// heading down
		else if (DirectionToMove == -1) {
			max = PlatformStartLocation.Z - DistanceToMove;
		}
	}
	// DEPTH //
	// if started up 
	else if (DepthStartDirection == EPlatformStartDirD::D_In) {
		// heading In
		if (DirectionToMove == 1) {
			max = PlatformStartLocation.Y + DistanceToMove;
		}
		// heading Out
		else if (DirectionToMove == -1) {
			max = PlatformStartLocation.Y;
		}
	}
	// if started down 
	else if (DepthStartDirection == EPlatformStartDirD::D_Out) {
		// heading In
		if (DirectionToMove == 1) {
			max = PlatformStartLocation.Y;
		}
		// heading Out
		else if (DirectionToMove == -1) {
			max = PlatformStartLocation.Y - DistanceToMove;
		}
	}
	return max;
}

void ALD_MovingPlatform::MoveHorizontal(float deltaTime) {
	FVector platformLocation = GetActorLocation();
	float distanceToMove = DirectionToMove * MovementSpeed * deltaTime;
	FVector finalLocation = platformLocation;
	finalLocation.X = platformLocation.X + distanceToMove;
	
	float maxLocation = LocationMaxDistance();	 
	
	// Started right, going right
	if (IsHeadingInStartDirection() && HorizontalStartDirection == EPlatformStartDirH::H_Right) {
		if (finalLocation.X >= maxLocation) {
			finalLocation.X = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}
	// Started right, going left
	else if (!IsHeadingInStartDirection() && HorizontalStartDirection == EPlatformStartDirH::H_Right) {
		if (finalLocation.X <= maxLocation) {
			finalLocation.X = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}
	// Started left, going left
	else if (IsHeadingInStartDirection() && HorizontalStartDirection == EPlatformStartDirH::H_Left) {
		if (finalLocation.X <= maxLocation) {
			finalLocation.X = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}	
	// Started left, going right
	else if (!IsHeadingInStartDirection() && HorizontalStartDirection == EPlatformStartDirH::H_Left) {
		if (finalLocation.X >= maxLocation) {
			finalLocation.X = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}

	SetActorLocation(finalLocation);
}

void ALD_MovingPlatform::MoveVertical(float deltaTime) {
	FVector platformLocation = GetActorLocation();
	float distanceToMove = DirectionToMove * MovementSpeed * deltaTime;
	FVector finalLocation = platformLocation;
	finalLocation.Z = platformLocation.Z + distanceToMove;

	float maxLocation = LocationMaxDistance();

	// Started up, going up
	if (IsHeadingInStartDirection() && VerticalStartDirection == EPlatformStartDirV::V_Up) {
		if (finalLocation.Z >= maxLocation) {
			finalLocation.Z = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	} 
	// Started up, going down
	else if (!IsHeadingInStartDirection() && VerticalStartDirection == EPlatformStartDirV::V_Up) {
		if (finalLocation.Z <= maxLocation) {
			finalLocation.Z = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}
	// Started down, going down
	else if (IsHeadingInStartDirection() && VerticalStartDirection == EPlatformStartDirV::V_Down) {
		if (finalLocation.Z <= maxLocation) {
			finalLocation.Z = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}
	// Started down, going up
	else if (!IsHeadingInStartDirection() && VerticalStartDirection == EPlatformStartDirV::V_Down) {
		if (finalLocation.Z >= maxLocation) {
			finalLocation.Z = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}

	SetActorLocation(finalLocation);
}

void ALD_MovingPlatform::MoveDepth(float deltaTime) {
	FVector platformLocation = GetActorLocation();
	float distanceToMove = DirectionToMove * MovementSpeed * deltaTime;
	FVector finalLocation = platformLocation;
	finalLocation.Y = platformLocation.Y + distanceToMove;

	float maxLocation = LocationMaxDistance();

	// Started in, going in
	if (IsHeadingInStartDirection() && DepthStartDirection == EPlatformStartDirD::D_In) {
		if (finalLocation.Y >= maxLocation) {
			finalLocation.Y = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}
	// Started in, going out
	else if (!IsHeadingInStartDirection() && DepthStartDirection == EPlatformStartDirD::D_In) {
		if (finalLocation.Y <= maxLocation) {
			finalLocation.Y = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}
	// Started out, going out
	else if (IsHeadingInStartDirection() && DepthStartDirection == EPlatformStartDirD::D_Out) {
		if (finalLocation.Y <= maxLocation) {
			finalLocation.Y = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}
	// started out, going in
	else if (!IsHeadingInStartDirection() && DepthStartDirection == EPlatformStartDirD::D_Out) {
		if (finalLocation.Y >= maxLocation) {
			finalLocation.Y = maxLocation;
			SetIsPausing(true);
			DirectionToMove *= -1;
		}
	}

	SetActorLocation(finalLocation);
}

const float pi = 3.14159;
void ALD_MovingPlatform::MoveRadial(float deltaTime) {
	//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, "LocationMaker" + lm.ToString());

	FVector point = GetActorLocation();
	FVector pivot = LocationMarker->GetComponentLocation();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, "Point: " + point.ToString());
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, "Pivot: " + pivot.ToString());
	
	// Convert RotationSpeed degrees to radians
	//float angle = deltaTime * RotationSpeed * (pi / 180.0);
	float angle = deltaTime * RotationSpeed;
	//FVector inputVector = pivot - point;
	//FVector yAxis = FVector(0, 1, 0);
	//FVector finalPos = UKismetMathLibrary::RotateAngleAxis(FVector(1,0,0), angle, yAxis);
	
	//float x = point.X - (point.X - pivot.X);
	//float z = point.X - (point.Z - pivot.Z);
	//
	////float rotatedX = FMath::Cos(angle) * (point.X - pivot.X) - FMath::Sin(angle) * (point.Z - pivot.Z) + pivot.X;
	//float rotatedX = point.X * FMath::Cos(angle) - FMath::Sin(angle) * (point.Z) + point.X - pivot.X;
	//float rotatedZ = point.X * FMath::Sin(angle) + FMath::Cos(angle) * (point.Z) + point.Z - pivot.Z;

	//float s = sin(deltaTime*RotationSpeed * (pi/180.0));
	//float c = cos(deltaTime*RotationSpeed * (pi/180.0));
	//
	//// Tranlate point back to origin
	//float x = point.X - pivot.X;
	//float z = point.Z - pivot.Z;
	//
	//// rotate point
	//float xnew = point.X * c - point.Z * s;
	//float znew = point.X * s + point.Z * c;
	//
	//// translate back

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, "FinalPos: " + finalPos.ToString());
	//
	//SetActorLocation(finalPos);
}

void ALD_MovingPlatform::MovePlatform(float deltaTime) {
	switch (Direction) {
	case EPlatformDirection::PO_Horizonatal:
		MoveHorizontal(deltaTime);
		break;
	case EPlatformDirection::PO_Vertical:
		MoveVertical(deltaTime);
		break;
	case EPlatformDirection::PO_Depth:
		MoveDepth(deltaTime);
		break;
	case EPlatformDirection::PO_Path:
		break;
	case EPlatformDirection::PO_Radial:
		MoveRadial(deltaTime);
		break;
	default:
		break;
	}
}

void ALD_MovingPlatform::ResetAllIsPlatDirExcept(EPlatformDirection dirToIgnore) {
	IsPlatDirH = false;
	IsPlatDirV = false;
	IsPlatDirD = false;
	IsPlatDirP = false;
	IsPlatDirR = false;

	dirToIgnore == EPlatformDirection::PO_Horizonatal ? IsPlatDirH = true :
		dirToIgnore == EPlatformDirection::PO_Vertical ? IsPlatDirV = true :
		dirToIgnore == EPlatformDirection::PO_Depth ? IsPlatDirD = true :
		dirToIgnore == EPlatformDirection::PO_Path ? IsPlatDirP = true :
		IsPlatDirR = true;
}

void ALD_MovingPlatform::ResetPlatDirDropdownsExcept(EPlatformDirection dirToIgnor) {
	switch (dirToIgnor) {
	case EPlatformDirection::PO_Horizonatal:
		VerticalStartDirection = EPlatformStartDirV::V_INVALID;
		DepthStartDirection = EPlatformStartDirD::D_INVALID;
		RadialDirection = EPlatformStartDirR::R_INVALID;
		break;
	case EPlatformDirection::PO_Vertical:
		HorizontalStartDirection = EPlatformStartDirH::H_INVALID;
		DepthStartDirection = EPlatformStartDirD::D_INVALID;
		RadialDirection = EPlatformStartDirR::R_INVALID;
		break;
	case EPlatformDirection::PO_Depth:
		HorizontalStartDirection = EPlatformStartDirH::H_INVALID;
		VerticalStartDirection = EPlatformStartDirV::V_INVALID;
		RadialDirection = EPlatformStartDirR::R_INVALID;
		break;
	case EPlatformDirection::PO_Path:
		HorizontalStartDirection = EPlatformStartDirH::H_INVALID;
		VerticalStartDirection = EPlatformStartDirV::V_INVALID;
		RadialDirection = EPlatformStartDirR::R_INVALID;
		DepthStartDirection = EPlatformStartDirD::D_INVALID;
		break;
	case EPlatformDirection::PO_Radial:
		HorizontalStartDirection = EPlatformStartDirH::H_INVALID;
		VerticalStartDirection = EPlatformStartDirV::V_INVALID;
		DepthStartDirection = EPlatformStartDirD::D_INVALID;
		break;
	default:
		break;
	}
}

void ALD_MovingPlatform::ToggleLocationMarkerVisibility(EPlatformDirection direction) {
	if (direction == EPlatformDirection::PO_Path || direction == EPlatformDirection::PO_Radial) {
		LocationMarker->SetVisibility(true);
	} else {
		LocationMarker->SetVisibility(false);
	}
}

void ALD_MovingPlatform::SetIsUsingDistance(EPlatformDirection direction) {
	if (direction == EPlatformDirection::PO_Horizonatal || 
		direction == EPlatformDirection::PO_Vertical	||
		direction == EPlatformDirection::PO_Depth) {
		IsUsingDistance = true;
	} else {
		IsUsingDistance = false;
		DistanceToMove = 0.0f;
	}
}