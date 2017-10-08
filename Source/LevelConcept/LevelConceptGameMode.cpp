// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "LevelConcept.h"
#include "LevelConceptGameMode.h"
#include "LevelConceptCharacter.h"

ALevelConceptGameMode::ALevelConceptGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/BPLD_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
