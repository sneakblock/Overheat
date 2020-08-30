/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "ComboSystemEditor.h"

class UEdGraph;
class UComboSystem;
struct Rect;

class COMBOSYSTEMEDITOR_API FComboSystemEditorUtilities
{
public:

	/** Can we paste to this graph? */
	static bool CanPasteNodes(const class UEdGraph* Graph);

	/** Perform paste on graph, at location */
	static void  PasteNodesHere(class UEdGraph* Graph, const FVector2D& Location);

	static TSharedPtr<FComboSystemEditor> GetComboSystemEditorForObject(const UObject* ObjectToFocusOn);

	FComboSystemEditorUtilities() {}
};
