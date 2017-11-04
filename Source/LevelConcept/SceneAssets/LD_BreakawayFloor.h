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

	// Whether or not the player has triggered the platform.
	bool IsTriggered;
	bool IsPlayerOverlapped;
	// Timer handle used when player jumps on platform.
	FTimerHandle BreakawayTimer;

	/***** WORLD *****/
	const UWorld* WorldPtr;

/************************* 
		METHODS 
*************************/
public:	
	ALD_BreakawayFloor();
	// Override to enable the initialization of the world pointer.
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	// Getters //
	UFUNCTION(BlueprintCallable, Category = "Breakaway")
	FORCEINLINE bool GetIsTriggered() const;
	FORCEINLINE const UWorld* GetWorldPtr() const;
	// Setters //
	void SetColliderExtents(EFloorSize sizeOfFloor);
	void SetColliderLocation(EFloorSize sizeOfFloor);
public:
	UFUNCTION(Category = "Collision")
	void FloorOverlapDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	void FloorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	virtual void BeginPlay() override;

private:
	void Breakaway();
	void Respawn();
	void StartTimerToBreakaway();
	void StartTimerToRespawn();
};
