/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/


#include "ComboSystemGraphNode.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Editor/EditorEngine.h"
#include "ComboSystemGraph.h"
#include "GraphEditorActions.h"
#include "ScopedTransaction.h"
#include "Framework/Commands/GenericCommands.h"
#include "ComboTree.h"
#include "Engine/Font.h"
#include "EdGraph/EdGraphNode.h"
#include "ComboSystemGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "ComboSystemGraphNode"

UComboSystemGraphNode::UComboSystemGraphNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

//main overide function
void UComboSystemGraphNode::SetComboNode(UComboNode* InComboNode, UEnum *ECombo)
{
	ComboNode = InComboNode;

	RemoveAllOutputPins();

	for(int i =0; i< ECombo->GetMaxEnumValue(); i++)
	{
		FName name = ECombo->GetNameByIndex(i);
		UComboSystemGraphNode_Base::CreateOutputPin(FText::FromName(name));
	}

	UComboSystemGraphNode_Base::CreateInputPin();
	InComboNode->GraphNode = this;
}

#undef LOCTEXT_NAMESPACE

COMBOSYSTEMEDITOR_API bool UComboSystemGraphNode::CanUserDeleteNode() const
{
	return true;
}

COMBOSYSTEMEDITOR_API void UComboSystemGraphNode::PostCopyNode()
{
	ResetComboNodeOwner();
}

void UComboSystemGraphNode::ResetComboNodeOwner()
{
	if (ComboNode)
	{
		UComboTree* ComboSystem = CastChecked<UComboSystemGraph>(GetGraph())->GetComboSystem();

		if (ComboNode->GetOuter() != ComboSystem)
		{
			// Ensures ComboNode is owned by the ComboSystem
			ComboNode->Rename(NULL, ComboSystem, REN_DontCreateRedirectors);
		}

		// Set up the back pointer for newly created combo nodes
		ComboNode->GraphNode = this;
	}
}

void UComboSystemGraphNode::GetContextMenuActions(const FGraphNodeContextMenuBuilder& Context) const
{
	if (Context.Node)
	{
		Context.MenuBuilder->BeginSection("ComboSystemGraphNodeEdit");
		{
			Context.MenuBuilder->AddMenuEntry(FGenericCommands::Get().Delete);
			Context.MenuBuilder->AddMenuEntry(FGenericCommands::Get().Cut);
			Context.MenuBuilder->AddMenuEntry(FGenericCommands::Get().Copy);
			Context.MenuBuilder->AddMenuEntry(FGenericCommands::Get().Duplicate);
		}
		Context.MenuBuilder->EndSection();

	}
}
