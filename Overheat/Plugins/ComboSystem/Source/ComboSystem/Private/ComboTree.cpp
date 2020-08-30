/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboTree.h"

//UObject Interface
#if WITH_EDITOR

FString UComboTree::GetDesc()
{
	return FString("Combo Tree");
}

void UComboTree::PostInitProperties()
{
	Super::PostInitProperties();
	//if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad)){}
}
void UComboTree::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UComboTree::AddReferencedObjects(UObject * InThis, FReferenceCollector & Collector)
{
	UComboTree* This = CastChecked<UComboTree>(InThis);

	Collector.AddReferencedObject(This->ComboSystemGraph, This);
	Collector.AddReferencedObject(This->RootNode, This);

	Super::AddReferencedObjects(InThis, Collector);
}

void UComboTree::PostLoad()
{
	Super::PostLoad();
}

#endif

/** EDITOR COMMANDS **/
#if WITH_EDITOR

UEdGraph* UComboTree::GetGraph()
{
	return ComboSystemGraph;
}

void UComboTree::SetGraph(UEdGraph *GraphEditor)
{
	ComboSystemGraph = GraphEditor;
}

#endif


