/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AssetData.h"
#include "EdGraph/EdGraphSchema.h"
#include "ComboSystemGraphSchema.generated.h"

class UEdGraph;

/** Action to add a node to the graph */
USTRUCT()
struct COMBOSYSTEMEDITOR_API FComboSystemGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	/** Class of node we want to create */
	class UClass* ComboNodeClass;

	static FName StaticGetTypeId() { static FName Type("FComboSystemGraphSchemaAction_NewNode"); return Type; }

	FComboSystemGraphSchemaAction_NewNode()
		: FEdGraphSchemaAction()
	{}

	FComboSystemGraphSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface

private:
	/** Connects new node to output of selected nodes */
	//void ConnectToSelectedNodes(UComboNode* NewNodeclass, UEdGraph* ParentGraph) const;

};


/** Action to add nodes to the graph based on selected objects*/
USTRUCT()
struct COMBOSYSTEMEDITOR_API FComboSystemGraphSchemaAction_NewFromSelected : public FComboSystemGraphSchemaAction_NewNode
{
	GENERATED_USTRUCT_BODY();
};

/** Action to create new comment */
USTRUCT()
struct COMBOSYSTEMEDITOR_API FComboSystemGraphSchemaAction_NewComment : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();
};

/** Action to paste clipboard contents into the graph */
USTRUCT()
struct COMBOSYSTEMEDITOR_API FComboSystemGraphSchemaAction_Paste : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	FComboSystemGraphSchemaAction_Paste()
		: FEdGraphSchemaAction()
	{}

	FComboSystemGraphSchemaAction_Paste(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface
};


UCLASS(MinimalAPI)
class UComboSystemGraphSchema : public UEdGraphSchema
{
	GENERATED_UCLASS_BODY()

	//use base for now

	//~ Begin EdGraphSchema Interface
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	//virtual void GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	//virtual bool ShouldHidePinDefaultValue(UEdGraphPin* Pin) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;
	//virtual void DroppedAssetsOnGraph(const TArray<struct FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const override;
	//virtual void DroppedAssetsOnNode(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraphNode* Node) const override;
	//virtual int32 GetNodeSelectionCount(const UEdGraph* Graph) const override;
	//virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;

	virtual void BreakSinglePinLink(UEdGraphPin *SourcePin, UEdGraphPin * TargetPin) const override;

	//~ End EdGraphSchema Interface
private:

	/** Adds action for creating a comment */
	void GetCommentAction(FGraphActionMenuBuilder& ActionMenuBuilder, const UEdGraph* CurrentGraph = NULL) const;

	/** A list of all available ComboNode classes - just root and regular */
	//static TArray<UClass*> ComboNodeClasses;

};
