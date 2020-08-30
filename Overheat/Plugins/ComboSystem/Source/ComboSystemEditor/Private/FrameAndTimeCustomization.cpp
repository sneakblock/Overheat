/*
* Copyright (c) 2019 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "FrameAndTimeCustomization.h"

//Copypaste from RangeStructCustomization
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SComboBox.h"
#include "DetailWidgetRow.h"
#include "Editor.h"
#include "PropertyHandle.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "SlateBasics.h"

#define LOCTEXT_NAMESPACE "ComboNodeClassDetails"

FrameAndTimeCustomization::FrameAndTimeCustomization(FComboSystemEditor *ComboSystemEditor)
	:ComboSystemEditor(ComboSystemEditor)
{

}

TSharedRef<IDetailCustomization> FrameAndTimeCustomization::MakeInstance(FComboSystemEditor *ComboSystemEditor)
{
	return MakeShareable(new FrameAndTimeCustomization(ComboSystemEditor));
}

void FrameAndTimeCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	if (!ComboSystemEditor)
		return;
	if (!ComboSystemEditor->GetComboTree())
		return;
	if (ComboSystemEditor->GetComboTree()->IsFrameIndependent)
	{
		DetailBuilder.HideProperty("StartupFrames");
		DetailBuilder.HideProperty("InputFrames");
		DetailBuilder.HideProperty("CooldownFrames");
	}
	else
	{
		DetailBuilder.HideProperty("StartupTime");
		DetailBuilder.HideProperty("InputTime");
		DetailBuilder.HideProperty("CooldownTime");
	}
}

#undef LOCTEXT_NAMESPACE