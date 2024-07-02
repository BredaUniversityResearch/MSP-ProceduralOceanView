// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MSP_ProceduralGenTarget : TargetRules
{
	public MSP_ProceduralGenTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange( new string[] { "MSP_ProceduralGen" } );
	}
}
