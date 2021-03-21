// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VivoxCore : ModuleRules
{
	public VivoxCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Private/VCorePluginPrivatePCH.h";

		bool bDisablePlugin = false;

        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            bDisablePlugin = true;
        }

		if (Target.Type == TargetRules.TargetType.Server)
        {
            bDisablePlugin = true;
        }

        if (bDisablePlugin)
        {
            PublicDefinitions.Add("DISABLE_VIVOXCORE=1");
        }
        else
        {
            PublicDefinitions.Add("DISABLE_VIVOXCORE=0");

            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "VivoxVoiceChat",
                "VoiceChat",
                "VivoxCoreSDK",
                "VivoxClientAPI"
            }
            );
        }

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"OnlineSubsystem",
				"OnlineSubsystemUtils",
				"Projects"
			}
			);


    }
}
