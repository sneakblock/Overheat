/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "ComboTree.h"
#include "ComboSystemGraph.generated.h"

UCLASS()
class COMBOSYSTEMEDITOR_API UComboSystemGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

	 class UComboTree* GetComboSystem() const;

	static UEdGraph* CreateNewComboSystemGraph(UComboTree* InComboSystem);

};

