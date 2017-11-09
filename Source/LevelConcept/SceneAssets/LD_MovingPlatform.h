// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LD_MovingPlatform.generated.h"

UENUM(BlueprintType)
enum class EPlatformDirection : uint8 {
	PO_INVALID = 0		UMETA(Hidden),
	PO_Horizonatal		UMETA(DisplayName = "Horizontal"),
	PO_Vertical			UMETA(DisplayName = "Vertical"),
	PO_Depth			UMETA(DisplayName = "Depth"),
	PO_Path				UMETA(DisplayName = "Path"),
	PO_Radial			UMETA(DisplayName = "Radial"),
	MAX					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EPlatformStartDirH : uint8 {
	H_INVALID = 0	UMETA(Hidden),
	H_Left			UMETA(DisplayName = "Left"),
	H_Right			UMETA(DisplayName = "Right"),
	MAX				UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EPlatformStartDirV : uint8 {
	V_INVALID = 0	UMETA(Hidden),
	V_Up			UMETA(DisplayName = "Up"),
	V_Down			UMETA(DisplayName = "Down"),
	MAX				UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EPlatformStartDirD : uint8 {
	D_INVALID = 0	UMETA(Hidden),
	D_In			UMETA(DisplayName = "In"),
	D_Out			UMETA(DisplayName = "Out"),
	MAX				UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EPlatformStartDirR : uint8 {
	R_INVALID = 0		UMETA(Hidden),
	R_Clockwise			UMETA(DisplayName = "Clockwise"),
	R_CounterClockWise	UMETA(DisplayName = "Counter Clockwise"),
	MAX					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EPlatformSpikeDir : uint8 {
	PSD_NONE = 0		UMETA(DisplayName = "None"),
	PSD_ABOVE			UMETA(DisplayName = "Above"),
	PSD_BELOW			UMETA(DisplayName = "Below")
};

USTRUCT()
struct LEVELCONCEPT_API FDebugRequiredInfo {
	GENERATED_USTRUCT_BODY()
public:
	EPlatformDirection Direction;
	EPlatformStartDirH HStartDir;
	EPlatformStartDirV VStartDir;
	EPlatformStartDirD DStartDir;
	float DistanceToMove;
	FVector PlatformLocation;
public:
	FDebugRequiredInfo();
	FDebugRequiredInfo(	EPlatformDirection dir,	EPlatformStartDirH startDirH, 
		EPlatformStartDirV startDirV, EPlatformStartDirD startDirD, float moveDistance, FVector platformLocation);
	void operator=(const FDebugRequiredInfo& value);

};

USTRUCT(blueprintable)
struct LEVELCONCEPT_API FDebugPlatformInfo {
	GENERATED_USTRUCT_BODY()
public: 
	UPROPERTY(EditAnywhere, Category = "Platform|DEBUG")
	bool IsDebugInfoEnabled;

	UPROPERTY(EditAnywhere)
	float DebugLineThickness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DebugLineMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DebugPlatformMesh;

	UPROPERTY(EditAnywhere, Category = "Color")
	UMaterialInstance* DebugPlatMat;

	UPROPERTY(EditAnywhere, Category = "Color")
	UMaterialInstance* DebugLineMat;
	
public:
	FDebugPlatformInfo();
	FVector GetDebugPlatformMeshLocation(FDebugRequiredInfo reqDebugInfo);
	float GetMiddlePointBA(float pointA, float pointB);
	FVector GetDebugLineLocation(FDebugRequiredInfo reqDebugInfo);
	void ScaleDebugLine(FDebugRequiredInfo reqDebugInfo);

private:
	float DebugLineLength;

};

UCLASS()
class LEVELCONCEPT_API ALD_MovingPlatform : public AActor {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Platform|DEBUG")
	FDebugPlatformInfo DebugInfo;

	UPROPERTY(EditAnywhere, Category = "Platform")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, Category = "Platform|Direction")
	EPlatformDirection Direction;

	UPROPERTY(EditAnywhere, Category = "Platform|Direction", meta = (EditCondition = "IsPlatDirH"))
	EPlatformStartDirH HorizontalStartDirection;

	UPROPERTY(EditAnywhere, Category = "Platform|Direction", meta = (EditCondition = "IsPlatDirV"))
	EPlatformStartDirV VerticalStartDirection;


	UPROPERTY(EditAnywhere, Category = "Platform|Direction", meta = (EditCondition = "IsPlatDirD"))
	EPlatformStartDirD DepthStartDirection;
	

	UPROPERTY(EditAnywhere, Category = "Platform|Direction", meta = (EditCondition = "IsPlatDirR"))
	EPlatformStartDirR RadialDirection;

	UPROPERTY(EditAnywhere, Category = "Platform|Movement", meta = (EditCondition = "IsUsingDistance"))
	float DistanceToMove;

	/*Distance to travel per second*/
	UPROPERTY(EditAnywhere, Category = "Platform|Movement")
	float MovementSpeed;
	/*Degress to rotate per second*/
	UPROPERTY(EditAnywhere, Category = "Platform|Movement", meta = (EditCondition = "IsPlatDirR"))
	float RotationSpeed;
	UPROPERTY(EditAnywhere, Category = "Platform|Movement")
	bool IsTriggerRequired;
	/*Do you want to have the platform hold it's position when it starts*/
	UPROPERTY(EditAnywhere, Category = "Platform|Movement")
	bool HasInitialHold;
	UPROPERTY(EditAnywhere, Category = "Platform|Movement", meta = (EditCondition = "HasInitialHold"))
	float InitialHoldDuration;
	/*Do you want the platform to pause for a moment
	when it reaches it's max distance*/
	UPROPERTY(EditAnywhere, Category = "Platform|Movement")
	bool DoesPlatformPause;
	UPROPERTY(EditAnywhere, Category = "Platform|Movement", meta = (EditCondition = "DoesPlatformPause"))
	float PauseDuration;

	UPROPERTY(EditAnywhere, Category = "Platform|Features")
	bool HasSpikes;

	UPROPERTY(EditAnywhere, Category = "Platform|Features", meta = (EditCondition = "HasSpikes"))
	EPlatformSpikeDir SpikeDirection;

	UStaticMeshComponent* LocationMarker;

private:
	
	bool HasTriggeredStart;

	FVector PlatformStartLocation;
	bool IsPausing;
	UPROPERTY(VisibleDefaultsOnly)
	bool IsPlatDirH;
	UPROPERTY(VisibleDefaultsOnly)
	bool IsPlatDirV;
	UPROPERTY(VisibleDefaultsOnly)
	bool IsPlatDirD;
	UPROPERTY(VisibleDefaultsOnly)
	bool IsPlatDirP;
	UPROPERTY(VisibleDefaultsOnly)
	bool IsPlatDirR;
	UPROPERTY(VisibleDefaultsOnly)
	bool IsUsingDistance;

	UMaterialInstance* DebugPlatMat;
	UMaterialInstanceDynamic* InstDynamicMat;
	int8 DirectionToMove;
	float CurrentRadialRotation;
	FTimerHandle PlatformPauseTimer;

	/***** WORLD *****/
	const UWorld* WorldPtr;
public:	
	ALD_MovingPlatform(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Platform")
	void StartPlatformMovement();
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	FORCEINLINE FDebugRequiredInfo GetDebugRequiredInfo() const;
	void SetIsPausing(bool value);
	void StartInitialHoldTimer();
	void StartPlatformPauseTimer();
	void ResetPlatformPauseTimer();
	FORCEINLINE const UWorld* GetWorldPtr() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	virtual void PostInitializeComponents() override;
	void InitDirectionToMove();
	bool IsHeadingInStartDirection();
	float LocationMaxDistance();
	void MoveHorizontal(float deltaTime);
	void MoveVertical(float deltaTime);
	void MoveDepth(float deltaTime);
	//TODO:MovePath();
	void MoveRadial(float deltaTime);
	void MovePlatform(float deltaTime);
	void ResetAllIsPlatDirExcept(EPlatformDirection dirToIgnore);
	void ResetPlatDirDropdownsExcept(EPlatformDirection dirToIgnor);
	void ToggleLocationMarkerVisibility(EPlatformDirection direction);
	void SetIsUsingDistance(EPlatformDirection direction);
};
