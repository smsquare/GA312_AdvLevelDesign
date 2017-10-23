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
	uint8 NumOfCyanKeys;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keys")
	uint8 NumOfPurpleKeys;
	UPROPERTY(VisibleAnywhere, Category = "Keys")
	uint8 NumOfYellowKeys;

public:
	FPlayerKeys();
	~FPlayerKeys();

	// Getters //
	FORCEINLINE int GetNumOfCyanKeys() const;
	FORCEINLINE int GetNumOfPurpleKeys() const;
	FORCEINLINE int GetNumOfYellowKeys() const;

//private:

};