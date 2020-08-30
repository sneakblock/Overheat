/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystemGraphNode_Base.h"
#include "ComboSystemGraphSchema.h"
#include "UnrealEd.h"
#include "EdGraph/EdGraphSchema.h"
#include "BlueprintActionFilter.h"
#include "ComboSystemGraphNode.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Editor/EditorEngine.h"
#include "ComboSystemGraph.h"
#include "GraphEditorActions.h"
#include "ScopedTransaction.h"
#include "Framework/Commands/GenericCommands.h"
#include "Engine/Font.h"
#include "Engine.h"
#include "ComboSystemGraphEditorCommands.h"
#include "MovementModeEnum.h"
#include "ComboTree.h"

UComboSystemGraphNode_Base::UComboSystemGraphNode_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UComboSystemGraphNode_Base::InitializeSnapshot(UComboTree *Parent)
{
	Parent->ConstructComboNode<UComboNode>(UComboNode::StaticClass(), false);

	ComboNodeSnapshot = NewObject<UComboNode>(Parent, UComboNode::StaticClass());
}

void UComboSystemGraphNode_Base::TakeSnapshot()
{
	if (!ComboNodeSnapshot)
		return; // ISSUE: IF COMBO TREE IS OLD, THIS WILL BE NULL - should I just make combotree referenced by each grph node? ie each grph node holds reference to a combotree, so we can construct our own nodes
	ComboNodeSnapshot->CopyState(ComboNode);
}

void UComboSystemGraphNode_Base::PostLoad()
{
	Super::PostLoad();

	if (ComboNode)
	{
		PinArray.Empty();

		UComboSystemGraphNode_Base *GraphNode = Cast<UComboSystemGraphNode_Base>(ComboNode->GraphNode);
		if (GraphNode)
		{
			for (UEdGraphPin *Pin : GraphNode->Pins)
			{
				if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
				{
					PinArray.Add(Pin);
				}
			}
		}
	}

}

COMBOSYSTEMEDITOR_API void UComboSystemGraphNode_Base::SetComboNode(UComboNode * InComboNode, UEnum * ECombo)
{
	return COMBOSYSTEMEDITOR_API void();
}

void UComboSystemGraphNode_Base::UpdateComboNodeEnum(FString InEnumString)
{
	ComboNode->UpdateEnumStruct(InEnumString); 

	RemoveAllOutputPins();
	ComboNode->Modify();

	for (TObjectIterator<UEnum> Itr; Itr; ++Itr)
	{
		FText nameText = FText::FromName(Itr->GetFName());
		FString nameString = nameText.ToString();

		if (nameString.Compare(InEnumString, ESearchCase::CaseSensitive) == 0)
		{
			for (int i = 0; i< Itr->GetMaxEnumValue(); i++)
			{
				FName name = Itr->GetNameByIndex(i);
				this->CreateOutputPin(FText::FromName(name));
			}
		}

		GetGraph()->NotifyGraphChanged();
	}
}

void UComboSystemGraphNode_Base::CreateInputPin()
{
	UEdGraphPin* NewPin = CreatePin(EGPD_Input, TEXT("ComboNode"), /*FString(), nullptr,*/ FName(*( ComboNode->GetInputPinName(GetInputCount()).ToString() )));
	if (NewPin->PinName.IsNone())
	{
		NewPin->PinFriendlyName = FText::FromString(TEXT("Input"));
	}
}

COMBOSYSTEMEDITOR_API void UComboSystemGraphNode_Base::CreateOutputPin(FText name)
{
	UEdGraphPin* NewPin = CreatePin(EGPD_Output, TEXT("ComboNode"), /*FString(), nullptr,*/ FName(*(ComboNode->GetInputPinName(GetInputCount()).ToString())));
	if (NewPin->PinName.IsNone())
	{
		NewPin->PinFriendlyName = name;
	}
	PinArray.Add(NewPin);
}

void UComboSystemGraphNode_Base::GetInputPins(TArray<class UEdGraphPin*>& OutInputPins)
{
	OutInputPins.Empty();

	for (int32 PinIndex = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (Pins[PinIndex]->Direction == EGPD_Input)
		{
			OutInputPins.Add(Pins[PinIndex]);
		}
	}
}

UEdGraphPin* UComboSystemGraphNode_Base::GetInputPin(int32 InputIndex)
{
	check(InputIndex >= 0 && InputIndex < GetInputCount());

	for (int32 PinIndex = 0, FoundInputs = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (Pins[PinIndex]->Direction == EGPD_Input)
		{
			if (InputIndex == FoundInputs)
			{
				return Pins[PinIndex];
			}
			else
			{
				FoundInputs++;
			}
		}
	}

	return NULL;
}

int32 UComboSystemGraphNode_Base::GetInputCount() const
{
	int32 InputCount = 0;

	for (int32 PinIndex = 0, FoundInputs = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (Pins[PinIndex]->Direction == EGPD_Input)
		{
			InputCount++;
		}
	}

	return InputCount;
}

TArray<UEdGraphPin*> UComboSystemGraphNode_Base::GetOutputPins()
{
	TArray<UEdGraphPin*> Outpins = TArray<UEdGraphPin *>();

	for (int32 PinIndex = 0, FoundInputs = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (Pins[PinIndex]->Direction == EGPD_Output)
		{
			Outpins.Add(Pins[PinIndex]);
		}
	}

	return Outpins;

}

int UComboSystemGraphNode_Base::GetPinIndex(UEdGraphPin *Pin)
{
	int32 InputCount = 0;

	for (int32 PinIndex = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (Pins[PinIndex] == Pin)
		{
			return PinIndex;
		}
	}

	return -1;
}

void UComboSystemGraphNode_Base::RemoveAllOutputPins()
{
	for (int32 PinIndex = PinArray.Num()-1; PinIndex >= 0; PinIndex--)
	{
		UEdGraphPin* Pin = PinArray[PinIndex];
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			this->RemovePin(Pin);
			this->OnPinRemoved(Pin);
			PinArray.RemoveAt(PinIndex);
			Pin->Modify();
		}
	}

	UEdGraphNode *UpcastedNode = Cast<UEdGraphNode>(this);
	for (int32 PinIndex = UpcastedNode->Pins.Num() - 1; PinIndex >= 0; PinIndex--)
	{
		UEdGraphPin *Pin = UpcastedNode->Pins[PinIndex];
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			UpcastedNode->RemovePin(Pin);
			UpcastedNode->OnPinRemoved(Pin);
			//UpcastedNode->Pins.RemoveAt(PinIndex);
			Pin->Modify();
		}
	}

}

void UComboSystemGraphNode_Base::InsertNewNode(UEdGraphPin* FromPin, UEdGraphPin* NewLinkPin, TSet<UEdGraphNode*>& OutNodeList)
{
	const UComboSystemGraphSchema* Schema = CastChecked<UComboSystemGraphSchema>(GetSchema());

	// The pin we are creating from already has a connection that needs to be broken. We want to "insert" the new node in between, so that the output of the new node is hooked up too
	UEdGraphPin* OldLinkedPin = FromPin->LinkedTo[0];
	check(OldLinkedPin);

	FromPin->BreakAllPinLinks();

	// Hook up the old linked pin to the first valid output pin on the new node
	for (int32 OutpinPinIdx = 0; OutpinPinIdx<Pins.Num(); OutpinPinIdx++)
	{
		UEdGraphPin* OutputExecPin = Pins[OutpinPinIdx];
		check(OutputExecPin);
		if (ECanCreateConnectionResponse::CONNECT_RESPONSE_MAKE == Schema->CanCreateConnection(OldLinkedPin, OutputExecPin).Response)
		{
			if (Schema->TryCreateConnection(OldLinkedPin, OutputExecPin))
			{
				OutNodeList.Add(OldLinkedPin->GetOwningNode());
				OutNodeList.Add(this);
			}
			break;
		}
	}

	if (Schema->TryCreateConnection(FromPin, NewLinkPin))
	{
		OutNodeList.Add(FromPin->GetOwningNode());
		OutNodeList.Add(this);
	}
}

void UComboSystemGraphNode_Base::AllocateDefaultPins()
{
	check(Pins.Num() == 0);

	CreateInputPins();

	if (!IsRootNode())
	{
//		CreatePin(EGPD_Output, TEXT("ComboNode"), TEXT(""), NULL, /*bIsArray=*/ false, /*bIsReference=*/ false, TEXT("Output"));
	}
}

void UComboSystemGraphNode_Base::ReconstructNode()
{

}

void UComboSystemGraphNode_Base::AutowireNewNode(UEdGraphPin* FromPin)
{
	if (FromPin != NULL)
	{
		const UComboSystemGraphSchema* Schema = CastChecked<UComboSystemGraphSchema>(GetSchema());

		TSet<UEdGraphNode*> NodeList;

		// auto-connect from dragged pin to first compatible pin on the new node
		for (int32 i = 0; i<Pins.Num(); i++)
		{
			UEdGraphPin* Pin = Pins[i];
			check(Pin);
			FPinConnectionResponse Response = Schema->CanCreateConnection(FromPin, Pin);
			if (ECanCreateConnectionResponse::CONNECT_RESPONSE_MAKE == Response.Response)
			{
				if (Schema->TryCreateConnection(FromPin, Pin))
				{
					NodeList.Add(FromPin->GetOwningNode());
					NodeList.Add(this);
				}
				break;
			}
			else if (ECanCreateConnectionResponse::CONNECT_RESPONSE_BREAK_OTHERS_A == Response.Response)
			{
				InsertNewNode(FromPin, Pin, NodeList);
				break;
			}
		}

		// Send all nodes that received a new pin connection a notification
		for (auto It = NodeList.CreateConstIterator(); It; ++It)
		{
			UEdGraphNode* Node = (*It);
			Node->NodeConnectionListChanged();
		}
	}
}

bool UComboSystemGraphNode_Base::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const
{
	return Schema->IsA(UComboSystemGraphSchema::StaticClass());
}

FText UComboSystemGraphNode_Base::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Combo Node"));
}

FLinearColor UComboSystemGraphNode_Base::GetNodeTitleColor() const
{
	//if(ComboNode->MovementMode == ECombo_MovementMode::ALL)
	//	return FLinearColor(50.0 / 255.0, 70.0 / 255.0, 250.0 / 255.0, 1);
	return FLinearColor(80.0 / 255.0, 110.0 / 255.0, 190.0 / 255.0, 1);//blueish
}
