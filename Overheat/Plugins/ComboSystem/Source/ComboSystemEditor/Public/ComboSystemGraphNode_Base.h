/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once
#include "EdGraph/EdGraphNode.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ComboTree.h"
#include "ComboSystemGraphNode_Base.generated.h"

class UEdGraphPin;
class UComboNode;

UCLASS(MinimalAPI)
class UComboSystemGraphNode_Base : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()

	/** The ComboNode this represents */
	UPROPERTY(VisibleAnywhere, instanced, Category = Combo)
	UComboNode* ComboNode;

	/** The ComboNode this represents */
	UPROPERTY(instanced)
	UComboNode* ComboNodeSnapshot;

	void InitializeSnapshot(UComboTree *Parent);

	void TakeSnapshot();

	/* set function that is overidden by node and root*/
	COMBOSYSTEMEDITOR_API void SetComboNode(UComboNode* InComboNode, UEnum *ECombo);

	/** Create all of the input pins required */
	virtual void CreateInputPins() {};

	/** Is this the undeletable root node */
	virtual bool IsRootNode() const { return false; }

	COMBOSYSTEMEDITOR_API void UpdateComboNodeEnum(FString EnumString);

	COMBOSYSTEMEDITOR_API void CreateInputPin();

	COMBOSYSTEMEDITOR_API void CreateOutputPin(FText name);

	/** Get the Output Pins (should only ever be more than one) */
	//COMBOSYSTEMEDITOR_API TArray<class UEdGraphPin*> GetOutputPins();
	/** Get all of the Input Pins */
	COMBOSYSTEMEDITOR_API void GetInputPins(TArray<class UEdGraphPin*>& OutInputPins);
	/** Get a single Input Pin via its index */
	COMBOSYSTEMEDITOR_API class UEdGraphPin* GetInputPin(int32 InputIndex);
	/** Get the current Input Pin count */
	COMBOSYSTEMEDITOR_API int32 GetInputCount() const;

	TArray<UEdGraphPin *> GetOutputPins();

	int GetPinIndex(UEdGraphPin *Pin);

	FString EnumString;

	TArray<UEdGraphPin*> PinArray;

	void RemoveAllOutputPins();

	/**
	* Handles inserting the node between the FromPin and what the FromPin was original connected to
	*
	* @param FromPin			The pin this node is being spawned from
	* @param NewLinkPin		The new pin the FromPin will connect to
	* @param OutNodeList		Any nodes that are modified will get added to this list for notification purposes
	*/
	void InsertNewNode(UEdGraphPin* FromPin, UEdGraphPin* NewLinkPin, TSet<UEdGraphNode*>& OutNodeList);

	#if WITH_EDITOR

	// UEdGraphNode interface.
	virtual void AllocateDefaultPins() override;
	virtual void ReconstructNode() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	// End of UEdGraphNode interface.
	#endif
	//// UObject interface
	virtual void PostLoad() override;
	//virtual void PostEditImport() override;
	//virtual void PostDuplicate(bool bDuplicateForPIE) override;
	//// End of UObject interface

public:
	TSharedPtr<FString> CurrentlySelectedEnum;
	
};