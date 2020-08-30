/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

using UnrealBuildTool;

public class ComboSystem : ModuleRules
{
	public ComboSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});

        PublicIncludePaths.AddRange(
            new string[]
            {
                "ComboSystem/Public"
            });

        PrivateIncludePaths.AddRange(
            new string[]
            {
                "ComboSystem/Private"
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "InputCore",
                "Engine",
                "ComboSystem"
            });

        PrivateIncludePathModuleNames.AddRange(
            new string[]
            {
                "Core"
            });

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            });
        
        if (Target.Type == TargetType.Editor)
        {
            // editor references
            PrivateDependencyModuleNames.Add("ComboSystemEditor");

            PublicIncludePaths.Add("ComboSystemEditor/Public");

            PrivateIncludePaths.Add("ComboSystemEditor/Public");
        }

    }
}
