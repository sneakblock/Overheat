/*
* Copyright (c) 2019 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "IDetailCustomization.h"
#include "Slate.h"
#include "Engine.h"
#include "PropertyHandle.h"
#include "ComboSystemEditor.h"

#pragma once

class FrameAndTimeCustomization : public IDetailCustomization
{
public:
	FrameAndTimeCustomization(FComboSystemEditor *ComboSystemEditor);

	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance(FComboSystemEditor *ComboSystemEditor);

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	//void SetStaticComboSystemEditor(FComboSystemEditor *ComboSystemEditor);

private:
	FComboSystemEditor *ComboSystemEditor;
};