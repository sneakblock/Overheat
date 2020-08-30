/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/ 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/UObjectIterator.h"
#if WITH_EDITOR
#include "EdGraph/EdGraphNode.h"
#endif
#include "MovementModeEnum.h"
#include "ComboNode.generated.h"

USTRUCT()
struct FEnumStruct 
{
	GENERATED_BODY()

public:

		// default constructor -- without parameters, so it will work inside other structures 
	// that need default initializer, such as TArray
	FEnumStruct()
		: ComboEnumString(TEXT("EComboDefault")), ComboEnum(NULL)
	{
		for (TObjectIterator<UEnum> Itr; Itr; ++Itr)
		{
			FText nameText = FText::FromName(Itr->GetFName());
			FString nameString = nameText.ToString();
			if (nameString.Compare(ComboEnumString, ESearchCase::CaseSensitive) == 0)
				ComboEnum = Cast<UEnum>(Itr->GetArchetype());
		}
	}

	FEnumStruct(const FString& InComboEnumString)
		: ComboEnumString(InComboEnumString), ComboEnum(NULL)
	{
		for (TObjectIterator<UEnum> Itr; Itr; ++Itr)
		{
			FText nameText = FText::FromName(Itr->GetFName());
			FString nameString = nameText.ToString();
			if (nameString.Compare(ComboEnumString, ESearchCase::CaseSensitive) == 0)
				ComboEnum = Cast<UEnum>(Itr->GetArchetype());
		}
	}

	// constructor that takes 3 parameters
	FEnumStruct(const FString& InComboEnumString, UEnum *InComboEnum)
		: ComboEnumString(InComboEnumString), ComboEnum(InComboEnum)
	{}

	// ComboName string can be used to identify the current active combo
	UPROPERTY(EditAnywhere, Category="ComboSystem")
		FString ComboEnumString;

	UPROPERTY(EditAnywhere, Category = "ComboSystem")
		UEnum *ComboEnum;
};

USTRUCT()
struct FTestPropertyType
{
	GENERATED_BODY()

	UPROPERTY()
		float Value;
};

USTRUCT()
struct FModeChildren
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<class UComboNode *> ChildNodes;
};

UCLASS()
class COMBOSYSTEM_API UComboNode : public UObject
{
	GENERATED_BODY()
	
	static const int32 MAX_ALLOWED_CHILD_NODES = 32;

public:
	//Node Properties

	// ComboName string can be used to identify the current active combo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ComboSystem")
		FString ComboName;

	// After moving to a new combo, this combo system cannot be advanced until 'Startup Frames' have passed. Check the Frame Rate Independent option in the Combo Tree menu to do time-based combos.
	UPROPERTY(EditAnywhere, Category = "ComboSystem", DisplayName = "Startup Frames")
		uint32 StartupFrames;

	// During this combo, the combo system will accept input for 'Input Frames' after startup frames have finished. Check the Frame Rate Independent option in the Combo Tree menu to do time-based combos.
	UPROPERTY(EditAnywhere, Category = "ComboSystem", DisplayName = "Input Frames")
		uint32 InputFrames;

	// If input frames expires without advancing to another combo, combo system will not accept input for 'Cooldown Frames', afterwards it will move back to the root. Check the Frame Rate Independent option in the Combo Tree menu to do time-based combos.
	UPROPERTY(EditAnywhere, Category = "ComboSystem", DisplayName = "Cooldown Frames")
		uint32 CooldownFrames;

	// After moving to a new combo, this combo system cannot be advanced until 'Startup Time' has passed.
	UPROPERTY(EditAnywhere, Category = "ComboSystem", DisplayName = "Startup Time (s)")
		float StartupTime;

	// During this combo, the combo system will accept input for 'Input Time' seconds after startup time has finished.
	UPROPERTY(EditAnywhere, Category = "ComboSystem", DisplayName = "Input Time (s)")
		float InputTime;

	// If input time expires without advancing to another combo, the combo system will not accept input for 'Cooldown Time' seconds, afterwards it will move back to the root combo.
	UPROPERTY(EditAnywhere, Category = "ComboSystem", DisplayName = "Cooldown Time (s)")
		float CooldownTime;

	// Combo Enum that generates the output pins for this node
	UPROPERTY(EditAnywhere, Category = "ComboSystem")
		FEnumStruct ComboStructEnum = FEnumStruct();

	// Multiplier float value associated with this combo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ComboSystem")
		float ComboMultiplier = 1.0;

	UPROPERTY()
		TMap<ECombo_MovementMode, FModeChildren> Combo_MovementModeMap;

	// Enum value that will be associated with this combo to branch to other combos
	UPROPERTY(BlueprintReadOnly, Category = "ComboSystem")
		uint8 InputEnumValue;

	// Movement mode that the combo system must be in to advance to this combo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ComboSystem")
		ECombo_MovementMode MovementMode = ECombo_MovementMode::ALL;

	// 3Vector that will be associated with this combo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ComboSystem")
		FVector LaunchForce;

	// 3Vector that will be associated with this combo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ComboSystem")
		FVector EnemyLaunchForce;

	// If true, then LandHit() must be called before AdvanceToNextCombo() will transition to another combo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ComboSystem")
		bool LandHitRequired;

	//get an array of children nodes
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		TArray<UComboNode *> GetChildNodes();

	//UPROPERTY(EditAnywhere, Category = "ComboSystem")
	//	float NewPropertyTest;

	void InitNode()
	{
		for (unsigned u = 0; u <= (unsigned)ECombo_MovementMode::ALL; u++)
		{
			Combo_MovementModeMap.Add((ECombo_MovementMode)u, FModeChildren());
		}
	}

	float NodePosY = 0;
	
	virtual bool NeedsLoadForServer() const override {return false;}

	UPROPERTY()
		UComboNode *ParentNode;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY()
		UEdGraphNode *GraphNode;
#endif

#if WITH_EDITOR

	void CopyState(UComboNode * const OtherNode);

	void SetChild(class UComboNode * ChildNode, uint8 InEnumValue);

	void RemoveChild(UComboNode *ChildNode);

	void RemoveAllChildren();

	inline static bool NodePosYPredicate(const UComboNode& NodeA, const UComboNode& NodeB)
	{
		return (NodeA.NodePosY < NodeB.NodePosY);
	}

	/** Get the name of a specific input pin */
	virtual FText GetInputPinName(int32 PinIndex) const { return FText::GetEmpty(); }

	/** Helper function to set the position of a combo node on a grid */
	void PlaceNode(int32 NodeColumn, int32 NodeRow, int32 RowCount); // TODO: can move to graphnode?

	void UpdateEnumStruct(FString EnumName);

	void Reorder();

#endif
};
