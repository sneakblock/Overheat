/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystemGraphNode_Root.h"
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

#define LOCTEXT_NAMESPACE "ComboSystemGraphNode_Root"

UComboSystemGraphNode_Root::UComboSystemGraphNode_Root(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UComboSystemGraphNode_Root::SetComboNode(UComboNode* InComboNode, UEnum *ECombo)
{
	ComboNode = InComboNode;

	RemoveAllOutputPins();

	for (int i = 0; i< ECombo->GetMaxEnumValue(); i++)
	{
		FName name = ECombo->GetNameByIndex(i);
		UComboSystemGraphNode_Base::CreateOutputPin(FText::FromName(name));
	}

	InComboNode->GraphNode = this;
}

COMBOSYSTEMEDITOR_API FText UComboSystemGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Root Node"));
}


COMBOSYSTEMEDITOR_API bool UComboSystemGraphNode_Root::CanUserDeleteNode() const
{
	return false;
}

FLinearColor UComboSystemGraphNode_Root::GetNodeTitleColor() const
{
	return FLinearColor(180.0 / 255.0, 60.0 / 255.0, 60.0 / 255.0, 1);//rgb
}

#undef LOCTEXT_NAMESPACE

