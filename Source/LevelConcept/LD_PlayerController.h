#pragma once

#include "GameFramework/PlayerController.h"
#include "LD_PlayerController.generated.h"

UCLASS()
class LEVELCONCEPT_API ALD_PlayerController : public APlayerController {
	GENERATED_BODY()
public:
	ALD_PlayerController();
	FVector GetPlayerAimingDirection();	
};
