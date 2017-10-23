#pragma once

#include "GameFramework/Actor.h"
#include "LD_SmallKey.generated.h"

UENUM(BlueprintType)
enum class EKeyColor : uint8 {
	KC_CYAN = 0		UMETA(DisplayName = "Cyan Key"),
	KC_PURPLE		UMETA(DisplayName = "Purple Key"),
	KC_YELLOW		UMETA(DisplayName = "Yellow Key"),
	KC_INVALID		UMETA(Hidden)
};

UCLASS()
class LEVELCONCEPT_API ALD_SmallKey : public AActor  {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	EKeyColor KeyColor;
	UPROPERTY(EditAnywhere, Category = "SmallKey")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, Category = "Mesh|SmallKey")
	UStaticMeshComponent* SmallKeyMesh;

public:	
	ALD_SmallKey();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Category = "Proximity")
	void PickupDetection(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
