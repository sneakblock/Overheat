/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ComboSystemGraphNode_Base.h"
#include "ComboSystemGraphNode_Root.generated.h"

UCLASS(MinimalAPI)
class UComboSystemGraphNode_Root : public UComboSystemGraphNode_Base
{
	GENERATED_UCLASS_BODY()
public:

	/** Set the ComboNode this represents (also assigns this to the ComboNode in Editor)*/
	COMBOSYSTEMEDITOR_API void SetComboNode(UComboNode* InComboNode, UEnum *ECombo);

	COMBOSYSTEMEDITOR_API virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const;

	COMBOSYSTEMEDITOR_API virtual bool CanUserDeleteNode() const override;

	virtual FLinearColor GetNodeTitleColor() const override;

};
