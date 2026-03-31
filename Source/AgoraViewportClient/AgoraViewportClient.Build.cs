// Created by Satheesh. Copyright 2020.
// https://twitter.com/ryanjon2040. Discord - ryanjon2040#5319
// Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040

using System;
using UnrealBuildTool;

public class AgoraViewportClient : ModuleRules
{
	public AgoraViewportClient(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.EngineDefault;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		
#if UE_4_26_OR_LATER
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "DeveloperSettings", "CommonUI" });
#else
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
#endif
		
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "RHI", "InputCore", "EngineSettings" } );
		
		if (Environment.GetEnvironmentVariable("YETI_STRICT_BUILD") == "true")
		{
			bUseUnity = false;
			PCHUsage = PCHUsageMode.NoPCHs;
			
			Console.WriteLine($"{ToString()} is using STRICT build.");
		}
		else
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		}
	}
}
