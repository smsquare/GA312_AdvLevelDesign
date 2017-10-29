#pragma once

#include "GameFramework/Actor.h"
#include "LD_BreakawayFloor.generated.h"

/***********
	ENUMS
*************/
UENUM(BlueprintType)
enum class EFloorSize : uint8 {
	FS_100 = 0		UMETA(DisplayName = "100"),
	FS_250			UMETA(DisplayName = "250"),
	FS_500			UMETA(DisplayName = "500"),
	FS_INVALID = 99	UMETA(Hidden)
};

//********************************************************* //
//						BREAKAWAY FLOOR						//
//**********************************************************//
UCLASS()
class LEVELCONCEPT_API ALD_BreakawayFloor : public AActor {
	GENERATED_BODY()

/********************** 
		VARIABLES 
*************************/
public:
	// The width of the floor tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	EFloorSize FloorSize;
	// The time it takes for the platform to breakaway, 
	// i.e. the amount of time the player can stand on before they fall.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float TimeToBreakaway;
	
	// The time it takes for the platform to respawn AFTER it has broken away.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float TimeToRespawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Collider")
	UBoxComponent* FloorCollider;

	// The mesh used for the floor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* FloorMesh;


private: 
	// Location the platform will appear at on respawn. This is the starting location.
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	FVector RespawnLocation;

/************************* 
		METHODS 
*************************/
public:	
	ALD_BreakawayFloor();
	void SetColliderExtents(EFloorSize);
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;	
};
