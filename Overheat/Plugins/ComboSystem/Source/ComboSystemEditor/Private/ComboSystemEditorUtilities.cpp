/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystemEditorUtilities.h"
#include "ComboSystemGraph.h"
#include "ComboSystemGraphNode.h"
#include "Toolkits/ToolkitManager.h"
#include "ComboTree.h"
#include "GraphEditor.h"


bool FComboSystemEditorUtilities::CanPasteNodes(const class UEdGraph* Graph)
{
	bool bCanPaste = false;
	TSharedPtr<FComboSystemEditor> ComboSystemEditor = GetComboSystemEditorForObject(Graph);
	if (ComboSystemEditor.IsValid())
	{
		bCanPaste = ComboSystemEditor->CanPasteNodes();
	}
	return bCanPaste;
}

void FComboSystemEditorUtilities::PasteNodesHere(class UEdGraph* Graph, const FVector2D& Location)
{
	TSharedPtr<FComboSystemEditor> ComboSystemEditor = GetComboSystemEditorForObject(Graph);
	if (ComboSystemEditor.IsValid())
	{
		ComboSystemEditor->PasteNodesHere(Location);
	}
}

TSharedPtr<FComboSystemEditor> FComboSystemEditorUtilities::GetComboSystemEditorForObject(const UObject* ObjectToFocusOn)
{
	check(ObjectToFocusOn);

	// Find the associated ComboSystem
	UComboTree* ComboSystem = Cast<const UComboSystemGraph>(ObjectToFocusOn)->GetComboSystem();

	TSharedPtr<FComboSystemEditor> ComboSystemEditor;
	if (ComboSystem != NULL)
	{
		TSharedPtr< IToolkit > FoundAssetEditor = FToolkitManager::Get().FindEditorForAsset(ComboSystem);
		if (FoundAssetEditor.IsValid())
		{
			ComboSystemEditor = StaticCastSharedPtr<FComboSystemEditor>(FoundAssetEditor);
		}
	}
	return ComboSystemEditor;
}
