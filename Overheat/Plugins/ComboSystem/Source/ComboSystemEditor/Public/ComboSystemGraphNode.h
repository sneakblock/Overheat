/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ComboSystemGraphNode_Base.h"
#include "ComboSystemGraphNode.generated.h"

UCLASS(MinimalAPI)
class UComboSystemGraphNode : public UComboSystemGraphNode_Base
{
	GENERATED_UCLASS_BODY()
public:
#if WITH_EDITOR
	///** Set the ComboNode this represents (also assigns this to the ComboNode in Editor)*/
	COMBOSYSTEMEDITOR_API void SetComboNode(UComboNode* InComboNode, UEnum *ECombo);

	COMBOSYSTEMEDITOR_API virtual bool CanUserDeleteNode() const override;

	/** Fix up the node's owner after being copied */
	COMBOSYSTEMEDITOR_API void PostCopyNode();

	/** Make sure the combonode is owned by the ComboSystem*/
	void ResetComboNodeOwner();

	void GetContextMenuActions(const FGraphNodeContextMenuBuilder & Context) const;
#endif
};
