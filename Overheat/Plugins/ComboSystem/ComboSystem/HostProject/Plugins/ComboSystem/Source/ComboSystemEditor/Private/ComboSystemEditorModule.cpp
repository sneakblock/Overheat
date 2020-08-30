/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystemEditorModule.h"
#include "AssetTypeActions_ComboSystem.h"
#include "AssetToolsModule.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
//#include "IPluginManager.h"
#include "Runtime/Projects/Public/Interfaces/IPluginManager.h"
//#include "SlateStyle.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyle.h"

IMPLEMENT_MODULE(FComboSystemEditorModule, ComboSystemEditor);

DEFINE_LOG_CATEGORY(ComboSystemEditor)

#define LOCTEXT_NAMESPACE "ComboSystemEditor"

#define PLUGIN_BRUSH(RelativePath,...) FSlateImageBrush(FComboTreeStyle::InContent(RelativePath,".png"),__VA_ARGS__)

FComboSystemEditorModule::FComboSystemEditorModule()
	: PropertyModule(FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor"))
{

}

void FComboSystemEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_ComboSystem));

	//asset thumbnail
	FComboTreeStyle *s = new FComboTreeStyle();
	s->Initialize();

	//property customization
	PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
}

void FComboSystemEditorModule::ShutdownModule()
{

}

//Icon stuff
static TSharedPtr<FSlateStyleSet> StyleSet;

void FComboTreeStyle::Initialize() {

	if (StyleSet.IsValid()) { return; }

	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon128x128(128.0f, 128.0f);

	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("ComboSystem"))->GetContentDir());

	StyleSet->Set("ClassIcon.ComboTree", new PLUGIN_BRUSH(TEXT("Icons/ComboTree_512x"), Icon16x16));
	StyleSet->Set("ClassThumbnail.ComboTree", new PLUGIN_BRUSH(TEXT("Icons/ComboTree_512x"), Icon128x128));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());

};

FString FComboTreeStyle::InContent(const FString &RelativePath, const ANSICHAR* Extension) {

	static FString Content = IPluginManager::Get().FindPlugin(TEXT("ComboSystem"))->GetContentDir();

	return (Content / RelativePath) + Extension;

}

FName FComboTreeStyle::GetStyleSetName() {
	static FName StyleName(TEXT("ComboTreeStyle"));
	return StyleName;
}

#undef LOCTEXT_NAMESPACE
