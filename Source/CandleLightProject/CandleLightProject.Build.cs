// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CandleLightProject : ModuleRules
{
	public CandleLightProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEd",
				"KismetCompiler",
				"BlueprintGraph"
			});
		}

		PublicIncludePaths.AddRange(new string[] {
			"CandleLightProject",
			"CandleLightProject/Variant_Platforming",
			"CandleLightProject/Variant_Platforming/Animation",
			"CandleLightProject/Variant_Combat",
			"CandleLightProject/Variant_Combat/AI",
			"CandleLightProject/Variant_Combat/Animation",
			"CandleLightProject/Variant_Combat/Gameplay",
			"CandleLightProject/Variant_Combat/Interfaces",
			"CandleLightProject/Variant_Combat/UI",
			"CandleLightProject/Variant_SideScrolling",
			"CandleLightProject/Variant_SideScrolling/AI",
			"CandleLightProject/Variant_SideScrolling/Gameplay",
			"CandleLightProject/Variant_SideScrolling/Interfaces",
			"CandleLightProject/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
