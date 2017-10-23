#pragma once

// Include 'EngineMinimal.h' so it will compile
#include "EngineMinimal.h"
#include "LD_SmallKey.h"
#include "LD_PlayerKeys.generated.h"

USTRUCT()
struct LEVELCONCEPT_API FPlayerKeys {
	GENERATED_USTRUCT_BODY()

	//TODO: MOVE THESE TO PRIVATE AFTER IT WORKS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keys")
	int NumOfCyanKeys;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keys")
	int NumOfPurpleKeys;
	UPROPERTY(VisibleAnywhere, Category = "Keys")
	int NumOfYellowKeys;

public:
	FPlayerKeys();
	~FPlayerKeys();

//private:

};
