// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BomberManJCCSIS457 : ModuleRules
{
	public BomberManJCCSIS457(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
