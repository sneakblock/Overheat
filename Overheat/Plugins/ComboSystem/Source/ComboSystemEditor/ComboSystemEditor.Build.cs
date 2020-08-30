/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

using UnrealBuildTool;

public class ComboSystemEditor: ModuleRules
{
	public ComboSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore"});

        PublicIncludePaths.AddRange(
            new string[]
            {
                "ComboSystemEditor/Public",
                "ComboSystem/Public"
            });

        PrivateIncludePaths.AddRange(
            new string[]
            {
                "ComboSystemEditor/Private",
                "ComboSystem/Public"
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AssetRegistry",
                "Toolbox",
                "Core",
                "CoreUObject",
                "InputCore",
                "Slate",
                "Engine",
                "AssetTools",
                "UnrealEd", // for FAssetEditorManager
                "KismetWidgets",
                "ApplicationCore", //for application specific things (CLIPBOARD, ETC...)
                "KismetCompiler",
                "BlueprintGraph",
                "GraphEditor",
                "Kismet",  // for FWorkflowCentricApplication
                "PropertyEditor",
                "EditorStyle",
                "SlateCore",
                "Sequencer",
                "DetailCustomizations",
                "Settings",
                "Projects", //IPluginManager
                "ComboSystemEditor",
                "ComboSystem"
            });

        PrivateIncludePathModuleNames.AddRange(
            new string[]
            {
                "Core",
                "SlateCore"
            });

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            });
    }
}
