// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LevelConcept : ModuleRules
{
	public LevelConcept(TargetInfo Target)
	{
        // Folders to work with:
        PrivateIncludePaths.AddRange(new string[] {
            "LevelConcept",
            "LevelConcept/BASE",
            "LevelConcept/Pickups",
            "LevelConcept/Player",
            "LevelConcept/Projectiles",
            "LevelConcept/SceneAssets",
            "LevelConcept/Weapons"
        });
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
