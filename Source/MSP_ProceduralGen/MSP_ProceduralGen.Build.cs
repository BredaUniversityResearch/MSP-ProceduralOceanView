// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MSP_ProceduralGen : ModuleRules
{
	public MSP_ProceduralGen(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"Json",
			"JsonUtilities",
			"InputCore",
			"MoviePlayer"
		});

        PrivateDependencyModuleNames.AddRange(new string[] {
			"ProceduralMeshComponent",
			"GeometryScriptingCore",
			"GeometryCore",
			"GeometryFramework",
			"Slate",
			"SlateCore",
			"EnhancedInput",
			"SkyCreatorPlugin",
			"HTTP",
			//PROJ and dependencies
			"PROJ",
			"SQLiteCore",
			//GeoTiff loading 
			"LibTiff", 
			"zlib", 
			"LibJpegTurbo",

			"HeadMountedDisplay", // for MotionControllerComponent
			"UMG" // for WidgetInteractionComponent
		});
	}
}
