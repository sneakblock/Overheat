/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystemGraph.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ComboTree.h"
#include "ComboSystemEditor.h"
#include "ComboSystemGraphSchema.h"

UComboSystemGraph::UComboSystemGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UComboTree* UComboSystemGraph::GetComboSystem() const
{
	return CastChecked<UComboTree>(GetOuter());
}

UEdGraph* UComboSystemGraph::CreateNewComboSystemGraph(UComboTree* InComboSystem)
{
	UEdGraph *NewGraph = FBlueprintEditorUtils::CreateNewGraph(InComboSystem, NAME_None, UComboSystemGraph::StaticClass(), UComboSystemGraphSchema::StaticClass());
	InComboSystem->SetGraph(NewGraph); 
	UComboSystemGraph *NewComboGraph = CastChecked<UComboSystemGraph>(NewGraph);
	NewComboGraph->bAllowDeletion = false;
	const UEdGraphSchema* Schema = NewGraph->GetSchema();
	Cast<UComboSystemGraphSchema>(Schema)->CreateDefaultNodesForGraph(*NewComboGraph);
	
	return CastChecked<UComboSystemGraph>(NewComboGraph);
}