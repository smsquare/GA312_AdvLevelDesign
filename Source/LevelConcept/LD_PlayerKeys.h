#pragma once

// Include 'EngineMinimal.h' so it will compile
#include "EngineMinimal.h"
#include "LD_SmallKey.h"
#include "LD_PlayerKeys.generated.h"

USTRUCT()
struct LEVELCONCEPT_API FPlayerKeys {
	GENERATED_USTRUCT_BODY()

public:
	FPlayerKeys();
	~FPlayerKeys();

private:
	int NumOfCyanKeys;
	int NumOfPurpleKeys;
	int NumOfYellowKeys;
};
