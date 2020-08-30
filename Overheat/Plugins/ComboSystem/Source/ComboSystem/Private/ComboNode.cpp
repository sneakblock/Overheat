/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboNode.h"
#include "UObject/UObjectIterator.h"

TArray<UComboNode *> UComboNode::GetChildNodes()
{
	TArray<UComboNode *> ChildCombos;
	for (unsigned u = 0; u <= (unsigned)ECombo_MovementMode::ALL; u++)
	{
		for (int i = 0; i<Combo_MovementModeMap[static_cast<ECombo_MovementMode>(u)].ChildNodes.Num(); i++)
		{
			ChildCombos.Add(Combo_MovementModeMap[static_cast<ECombo_MovementMode>(u)].ChildNodes[i]);
		}
	}
	return ChildCombos;
}


#if WITH_EDITOR

void UComboNode::CopyState(UComboNode * const OtherNode)
{
	if (!OtherNode)
		return;
	this->ComboName = OtherNode->ComboName;
	this->StartupFrames = OtherNode->StartupFrames;
	this->InputFrames = OtherNode->InputFrames;
	this->CooldownFrames = OtherNode->CooldownFrames;
	this->StartupTime = OtherNode->StartupTime;
	this->InputTime = OtherNode->InputTime;
	this->CooldownTime = OtherNode->CooldownTime;
	this->ComboStructEnum = OtherNode->ComboStructEnum;
	this->ComboMultiplier = OtherNode->ComboMultiplier;
	this->InputEnumValue = OtherNode->InputEnumValue;
	this->MovementMode = OtherNode->MovementMode;
	this->LaunchForce = OtherNode->LaunchForce;
	this->EnemyLaunchForce = OtherNode->EnemyLaunchForce;
	this->LandHitRequired = OtherNode->LandHitRequired;
}

void UComboNode::SetChild(UComboNode * ChildNode, uint8 InEnumValue)
{
	//update with new child
	ChildNode->InputEnumValue = InEnumValue;
	ChildNode->ParentNode = this;

	ECombo_MovementMode ChildMovementMode = ChildNode->MovementMode;
	Combo_MovementModeMap[ChildMovementMode].ChildNodes.Add(ChildNode);
	Combo_MovementModeMap[ChildMovementMode].ChildNodes.Sort(UComboNode::NodePosYPredicate);
}

void UComboNode::RemoveChild(UComboNode *ChildNode)
{
	if (Combo_MovementModeMap.Num() < 1)
	{
		return;
	}

	ECombo_MovementMode ChildMovementMode = ChildNode->MovementMode;
	if (Combo_MovementModeMap[ChildMovementMode].ChildNodes.Contains(ChildNode))
	{
		Combo_MovementModeMap[ChildMovementMode].ChildNodes.Remove(ChildNode);
	}

}

void UComboNode::RemoveAllChildren()
{
	for (auto& Mode : Combo_MovementModeMap)
	{
		TArray<UComboNode *> Children = Mode.Value.ChildNodes;
		for (UComboNode *Child : Children)
		{
			Child->InputEnumValue = (uint8)ECombo_MovementMode::ALL;
			Child->ParentNode = nullptr;
		}
	}
	Combo_MovementModeMap.Empty();
}

void UComboNode::PlaceNode(int32 NodeColumn, int32 NodeRow, int32 RowCount)
{
	GraphNode->NodePosX = (-150 * NodeColumn) - 100;
	GraphNode->NodePosY = (100 * NodeRow) - (50 * RowCount);
	NodePosY = GraphNode->NodePosY;
}

void UComboNode::UpdateEnumStruct(FString EnumName)
{
	ComboStructEnum = FEnumStruct(EnumName);

	//ComboStructEnum->UpdateEnum(EnumName);

	//for (TObjectIterator<UEnum> Itr; Itr; ++Itr)
	//{
	//	FText nameText = FText::FromName(Itr->GetFName());
	//	FString nameString = nameText.ToString();

	//	if (nameString.Compare(EnumName, ESearchCase::CaseSensitive) == 0)
	//	{
	//		ComboStructEnum = FEnumStruct();
	//		ComboStructEnum.ComboEnumString = EnumName;
	//		ComboStructEnum.ComboEnum = Cast<UEnum>(Itr->GetArchetype());
	//	}
	//}
}

void UComboNode::Reorder()
{
	Combo_MovementModeMap[MovementMode].ChildNodes.Sort(UComboNode::NodePosYPredicate);
}


#endif