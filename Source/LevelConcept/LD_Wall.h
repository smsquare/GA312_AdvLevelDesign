#pragma once

#include "GameFramework/Actor.h"
#include "LD_Wall.generated.h"

UENUM(BlueprintType)
enum class EWallFrictionType : uint8 {
	WFT_Rough = 0	UMETA(DisplayName = "Rough"),
	WFT_Slippery	UMETA(DisplayName = "Slippery"),
	WFT_Smooth		UMETA(DisplayName = "Smooth"),
	WFT_Sticky		UMETA(DisplayName = "Sticky"),
	MAX				UMETA(Hidden)
};

USTRUCT()
struct LEVELCONCEPT_API FWallInformation {
	GENERATED_USTRUCT_BODY()
public:
	// The initial velocity for when you start sliding down the wall
	float InitSlideVelocity;
	// The acceleration of your slide on this walls surface
	float SlideAcceration;
	// The amount of time you can hold onto the wall before you start to slide
	float WallHoldDuration;

	FWallInformation() {
		InitSlideVelocity = 1.0f;
		SlideAcceration = 2.0f;
		WallHoldDuration = 1.0f;
	}
	FWallInformation(float InitVelocity, float Acceleration, float HoldDuration)
		: InitSlideVelocity(InitVelocity), SlideAcceration(Acceleration), WallHoldDuration(HoldDuration) {}

	void operator=(const FWallInformation& value) {
		InitSlideVelocity = value.InitSlideVelocity;
		SlideAcceration = value.SlideAcceration;
		WallHoldDuration = value.WallHoldDuration;
	}
};

UENUM(BlueprintType)
enum class ESecretWallDamageType : uint8 {
	SD_PLAYERKICK = 0		UMETA(DisplayName = "Player Kick"),
	SD_PLAYERPROJECTILE		UMETA(DisplayName = "Player Projectile"),
	SD_ENEMYPROJECTILE		UMETA(DisplayName = "Enemy Projectile"),
	SD_BOTHPROJECTILE		UMETA(DisplayName = "Player AND Enemy Projecilte"),
	SD_INVALID = 99			UMETA(Hidden)
};

UCLASS()
class LEVELCONCEPT_API ALD_Wall : public AActor {
	GENERATED_BODY()
	
public:	
	static const FWallInformation RoughWallInfo;
	static const FWallInformation SlipperyWallInfo;
	static const FWallInformation SmoothWallInfo;
	static const FWallInformation StickyWallInfo;

	// Mesh that contains the actual visible wall
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Wall Mesh")
	class UInstancedStaticMeshComponent* WallMesh;

	// Capsule to be placed in the scene for the location the player should fall off the wall.
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Fall Off Point")
	class UCapsuleComponent* FallOffPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
	EWallFrictionType WallType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall|Secret")
	bool IsSecretWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall|Secret", meta = (EditCondition = "IsSecretWall"))
	ESecretWallDamageType SecretDamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall|Secret", meta = (EditCondition = "IsSecretWall"))
	bool HasSecretMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall|Secret", meta = (EditCondition = "HasSecretMessage"))
	FString SecretMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall|Secret|Timer")
	FTimerHandle SecretMessageTimer;
public:
	ALD_Wall();
	virtual void BeginPlay() override;
	FWallInformation GetWallInfo();
	UFUNCTION(BlueprintCallable, Category = "Proximity")
	void SecretWallHit();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Secret")
	void ShowSecretWallUI();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Secret")
	void ResetSecretWallUI();
	UFUNCTION(Category = "Proximity")
	void FallOffDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Category = "Proximity")
	void FallOffEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Primarily a debug function, used to print the name of the Enum established at the top of this .h
	static FORCEINLINE FString GetEnumValueToString(const FString& EnumName, EWallFrictionType EnumValue);
private:
	void WallHit(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};