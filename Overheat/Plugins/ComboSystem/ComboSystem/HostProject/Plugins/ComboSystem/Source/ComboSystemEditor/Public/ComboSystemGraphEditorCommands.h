/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"

class FComboSystemGraphEditorCommands : public TCommands<FComboSystemGraphEditorCommands>
{
public:
	/** Constructor */
	FComboSystemGraphEditorCommands()
		: TCommands<FComboSystemGraphEditorCommands>("ComboSystemGraphEditor", NSLOCTEXT("Contexts", "ComboSystemGraphEditor", "ComboSystem Graph Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	TSharedPtr<FUICommandInfo> SetCombo;

	/** Initialize commands */
	virtual void RegisterCommands() override;
};
