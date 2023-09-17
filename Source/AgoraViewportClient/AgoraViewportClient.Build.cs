// Created by Satheesh. Copyright 2020.
// https://twitter.com/ryanjon2040. Discord - ryanjon2040#5319
// Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040

using UnrealBuildTool;

public class AgoraViewportClient : ModuleRules
{
	public AgoraViewportClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

#if UE_4_26_OR_LATER
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "DeveloperSettings" });
#else
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
#endif
		
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "RHI", "InputCore", "EngineSettings" } );
	}
}
