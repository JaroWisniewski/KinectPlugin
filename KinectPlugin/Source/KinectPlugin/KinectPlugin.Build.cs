// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class KinectPlugin : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "ThirdParty")); }
    }

    private string LibraryPath()
    {
        string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
        return Path.GetFullPath(Path.Combine(ThirdPartyPath, "Lib", PlatformString));
    }
    public KinectPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicAdditionalLibraries.Add(Path.Combine(LibraryPath(), "Kinect20.lib"));

        PublicIncludePaths.AddRange(
			new string[] {
				"KinectPlugin/Public",
                Path.Combine(ThirdPartyPath, "include")
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"KinectPlugin/Private",
                 Path.Combine(ThirdPartyPath, "include")
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
                "CoreUObject",
                "Core",
                "Engine",
                "Slate",
                "SlateCore",
                "RHI",
                "RenderCore",
                "ShaderCore",
                "HeadMountedDisplay",
                "InputCore"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
