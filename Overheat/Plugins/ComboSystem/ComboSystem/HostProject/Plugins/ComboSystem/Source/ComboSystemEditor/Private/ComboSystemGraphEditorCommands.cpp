/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystemGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "ComboSystemGraphEditorCommands"

void FComboSystemGraphEditorCommands::RegisterCommands()
{
	UI_COMMAND(SetCombo, "Set Enum", "Doesn't do anything at the moment", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
