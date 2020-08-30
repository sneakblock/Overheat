/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyle.h"

#include "PropertyEditorModule.h"
#include "PropertyEditorDelegates.h"
#include "DetailCustomizations.h"

#include "FrameAndTimeCustomization.h"

DECLARE_LOG_CATEGORY_EXTERN(ComboSystemEditor, All, All)

class FComboSystemEditorModule : public IModuleInterface
{
public:
	FComboSystemEditorModule();
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	FPropertyEditorModule& PropertyModule;
};

class FComboTreeStyle {

public:

	static FString InContent(const FString &RelativePath, const ANSICHAR* Extension);
	static FName GetStyleSetName();
	void Initialize();

};