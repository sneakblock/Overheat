/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystemGraphSchema.h"
#include "ScopedTransaction.h"
#include "ComboTree.h"
#include "ComboNode.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"
#include "ComboSystemGraph.h"
#include "ComboSystemGraphNode_Root.h"
#include "ComboSystemGraphNode_Base.h"
#include "ComboSystemEditorUtilities.h"

#define LOCTEXT_NAMESPACE "ComboSystemSchema"

UEdGraphNode* FComboSystemGraphSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode/* = true*/)
{
	check(ComboNodeClass);

	UComboTree* ComboSystem = CastChecked<UComboTree>(ParentGraph->GetOuter());

	UComboNode *NewNode = ComboSystem->ConstructComboNode<UComboNode>();
	if (FromPin)
	{
		UComboNode *ParentNode = Cast<UComboSystemGraphNode_Base>(FromPin->GetOwningNode())->ComboNode;
		NewNode->ParentNode = ParentNode;
	}
	FComboSystemEditor::SetupGraphNode(ComboSystem, NewNode, false);

	////const FScopedTransaction Transaction(LOCTEXT("ComboSystemEditorNewComboNode", "Combo System Editor: New Combo Node"));
	ParentGraph->Modify();
	ComboSystem->Modify();

	NewNode->GraphNode->NodePosX = Location.X;
	NewNode->GraphNode->NodePosY = Location.Y;
	NewNode->GraphNode->AutowireNewNode(FromPin);

	return NewNode->GraphNode;
}

UEdGraphNode* FComboSystemGraphSchemaAction_Paste::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode/* = true*/)
{
	FComboSystemEditorUtilities::PasteNodesHere(ParentGraph, Location);
	return NULL;
}

UComboSystemGraphSchema::UComboSystemGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UComboSystemGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	UComboTree* ComboSystem = CastChecked<UComboTree>(Graph.GetOuter());
	////const FScopedTransaction Transaction(LOCTEXT("ComboSystemEditorNewComboNode", "Combo System Editor: New Combo Node"));
	Graph.Modify();
	ComboSystem->Modify();
	UComboNode* ResultNode = ComboSystem->ConstructComboNode<UComboNode>();
	//editor.setup
	FComboSystemEditor::SetupRootGraphNode(ComboSystem, ResultNode, false);
	ComboSystem->RootNode = ResultNode;
	const int32 RootNodeHeightOffset = -58;

	// Create the result node
	ResultNode->GraphNode->NodePosY = RootNodeHeightOffset;
	SetNodeMetaData(ResultNode->GraphNode, FNodeMetadata::DefaultGraphNode);
}

FLinearColor UComboSystemGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

void UComboSystemGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder & ContextMenuBuilder) const
{
	const FText Name = LOCTEXT("NewCombo", "New Combo Node");
	const FText ToolTip = LOCTEXT("NewComboTooltip", "Create a new combo node");

	TSharedPtr<FComboSystemGraphSchemaAction_NewNode> NewActionNew(new FComboSystemGraphSchemaAction_NewNode(FText::GetEmpty(), Name, ToolTip, 0)); //register 'new combo' action

	NewActionNew->ComboNodeClass = UComboNode::StaticClass();

	ContextMenuBuilder.AddAction(NewActionNew);

	const FText Paste_Name = LOCTEXT("PasteNode", "Paste Combo Node");
	const FText Paste_ToolTip = LOCTEXT("PasteTooltip", "Paste a combo node");
	if (!ContextMenuBuilder.FromPin && FComboSystemEditorUtilities::CanPasteNodes(ContextMenuBuilder.CurrentGraph))
	{
		TSharedPtr<FComboSystemGraphSchemaAction_Paste> NewActionPaste(new FComboSystemGraphSchemaAction_Paste(FText::GetEmpty(), Paste_Name, Paste_ToolTip, 0));
		ContextMenuBuilder.AddAction(NewActionPaste);
	}

}

const FPinConnectionResponse UComboSystemGraphSchema::CanCreateConnection(const UEdGraphPin * PinA, const UEdGraphPin * PinB) const
{
	// Make sure the pins are not on the same node
	if (PinA->GetOwningNode() == PinB->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Both are on the same node"));
	}

	// Compare the directions
	const UEdGraphPin* InputPin = NULL;
	const UEdGraphPin* OutputPin = NULL;

	if (!CategorizePinsByDirection(PinA, PinB, /*out*/ InputPin, /*out*/ OutputPin))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionIncompatible", "Directions are not compatible"));
	}

	// Break existing connections on inputs only - multiple output connections are acceptable
	if (InputPin->LinkedTo.Num() > 0)
	{
		ECanCreateConnectionResponse ReplyBreakOutputs;
		if (InputPin == PinA)
		{
			ReplyBreakOutputs = CONNECT_RESPONSE_BREAK_OTHERS_A;
		}
		else
		{
			ReplyBreakOutputs = CONNECT_RESPONSE_BREAK_OTHERS_B;
		}
		return FPinConnectionResponse(ReplyBreakOutputs, LOCTEXT("ConnectionReplace", "Replace existing connections"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

bool UComboSystemGraphSchema::TryCreateConnection(UEdGraphPin * PinA, UEdGraphPin * PinB) const
{

	bool bModified = UEdGraphSchema::TryCreateConnection(PinA, PinB);

	if (bModified && 0)
	{
		//one output per pin
		if (PinA->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			//connecting input to output; remove all connections from output pin
			for (UEdGraphPin *ConnectedPin : PinB->LinkedTo)
			{			
				//remove any of PinB's connections that aren't to PinA
				if (ConnectedPin != PinA)
				{
					BreakSinglePinLink(PinB, ConnectedPin);
				}
			}
		}
		else if (PinA->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			//connecting output to input; remove all connections from output pin
			for (UEdGraphPin *ConnectedPin : PinA->LinkedTo)
			{
				//remove any of PinB's connections that aren't to PinA
				if (ConnectedPin != PinB)
				{
					BreakSinglePinLink(PinA, ConnectedPin);
				}
			}
		}	
	}

	return bModified;
}

void UComboSystemGraphSchema::BreakSinglePinLink(UEdGraphPin * SourcePin, UEdGraphPin * TargetPin) const
{
	UComboNode *SourceNode = Cast<UComboSystemGraphNode_Base>(SourcePin->GetOwningNode())->ComboNode;
	UComboNode *TargetNode = Cast<UComboSystemGraphNode_Base>(TargetPin->GetOwningNode())->ComboNode;
	SourceNode->RemoveChild(TargetNode);
	TargetNode->ParentNode = nullptr;

	UEdGraphSchema::BreakSinglePinLink(SourcePin, TargetPin);
}

void UComboSystemGraphSchema::BreakNodeLinks(UEdGraphNode & TargetNode) const
{
	UComboNode *ComboNode = Cast<UComboSystemGraphNode_Base>(& TargetNode)->ComboNode;
	ComboNode->RemoveAllChildren();
	if (ComboNode->ParentNode)
	{
		//ComboNode is no longer a child of ParentNode
		ComboNode->ParentNode->RemoveChild(ComboNode);
	}

	UEdGraphSchema::BreakNodeLinks(TargetNode);
}

void UComboSystemGraphSchema::BreakPinLinks(UEdGraphPin & TargetPin, bool bSendsNodeNotifcation) const
{
	TArray<UEdGraphPin *> ConnectedPins = TargetPin.LinkedTo;
	UComboNode *PinNode = Cast<UComboSystemGraphNode_Base>(TargetPin.GetOwningNode())->ComboNode;
	
	for (UEdGraphPin *Pin : ConnectedPins)
	{
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Input) // a child of Target Pin's node
		{
			UComboNode *ChildNode = Cast<UComboSystemGraphNode_Base>(Pin->GetOwningNode())->ComboNode;
			PinNode->RemoveChild(ChildNode);
		}
		else if (Pin->Direction == EEdGraphPinDirection::EGPD_Output) // a parent of Target Pin's node
		{
			UComboNode *ParentNode = Cast<UComboSystemGraphNode_Base>(Pin->GetOwningNode())->ComboNode;
			ParentNode->RemoveChild(PinNode);
		}
	}

	UEdGraphSchema::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UComboSystemGraphSchema::GetCommentAction(FGraphActionMenuBuilder & ActionMenuBuilder, const UEdGraph * CurrentGraph) const
{
}

#undef LOCTEXT_NAMESPACE