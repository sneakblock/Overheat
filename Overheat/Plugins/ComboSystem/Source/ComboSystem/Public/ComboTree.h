/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ComboNode.h"
#if WITH_EDITOR
#include "UnrealEd.h"
#include "Editor/UnrealEd/Public/Toolkits/AssetEditorToolkit.h"
#endif
#include "ComboTree.generated.h"

UENUM(BlueprintType, Category = "Combo")
enum class EComboDefault : uint8 {
	OUTPUT_A			UMETA(DisplayName = "First Output"),
	OUTPUT_B			UMETA(DisplayName = "Second Output")
};


UCLASS()
class COMBOSYSTEM_API UComboTree : public UObject
{
	GENERATED_BODY()
	
#if WITH_EDITORONLY_DATA

		UPROPERTY()
		class UEdGraph *ComboSystemGraph;

#endif

public:
	UPROPERTY()
		TArray<UComboNode *> AllNodes;

	UPROPERTY()
		UComboNode *RootNode;

	UPROPERTY()
		bool IsFrameIndependent;

#if WITH_EDITOR
		//~ Begin UObject Interface.
		virtual FString GetDesc() override;
		virtual void PostInitProperties() override;
		virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
		static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);
		virtual void PostLoad() override;
#endif
		//	virtual void Serialize(FArchive& Ar) override;
		//~ End UObject Interface.

#if WITH_EDITOR
		/** Construct and initialize a node within this ComboSystem */
		template<class T>
		T* ConstructComboNode(TSubclassOf<UComboNode> ComboNodeClass = T::StaticClass(), bool bSelectNewNode = true)
		{
			T* ComboNode = NewObject<T>(this, ComboNodeClass);
			AllNodes.Add(ComboNode);
			return ComboNode;
		}

#endif
	
#if WITH_EDITOR
public:
		/** Get the EdGraph of ComboNodes */
		class UEdGraph *GetGraph();

		/** Set the EdGraph of ComboNodes */
		void SetGraph(class UEdGraph*);
#endif

};
