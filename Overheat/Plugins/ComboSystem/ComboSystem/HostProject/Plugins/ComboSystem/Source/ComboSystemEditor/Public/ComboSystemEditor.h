/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "ComboTree.h"
#include "Editor/UnrealEd/Public/Toolkits/AssetEditorToolkit.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectBase.h"
#include "Misc/NotifyHook.h"
#include "CoreMinimal.h"
#include "ComboSystemGraph.h"
#include "EditorUndoClient.h"
#include "ComboSystemGraphNode_Base.h"
#include "Runtime/SlateCore/Public/Styling/SlateTypes.h"

class SGraphEditor;
class SDockableTab;

class COMBOSYSTEMEDITOR_API FComboSystemEditor : public FAssetEditorToolkit, public FGCObject, public FNotifyHook, public FEditorUndoClient
{
public:
	FComboSystemEditor();
	~FComboSystemEditor();

	void InitComboSystemEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UObject* ObjectToEdit);

	/** FGCObject interface */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	//~ Begin FEditorUndoClient Interface
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override { PostUndo(bSuccess); }
	// End of FEditorUndoClient

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual	void SaveAsset_Execute();
	virtual	void SaveAssetAs_Execute();

	/** tabs*/
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	virtual void FocusWindow(UObject *ObjectToFocusOn) override;
	virtual void InvokeTab(const FTabId & TabId) override;

	void OnTabForegrounded(TSharedPtr<SDockTab> ForegroundedTab, TSharedPtr<SDockTab> BackgroundedTab);

	FDelegateHandle TabForegroundedDelegateHandle;

	TSet<UObject*> GetSelectedNodes_WithoutRoot();

	TSet<UObject*> GetSelectedNodes();

	void ReselectNodes();

	/** set the current object **/
	void SetSelection(TArray<UObject*> SelectedObjects);

	void BindGraphCommands();

	void SetCombo();

	void SetCurrentSelectionEnum(FString enum_name);

	typedef void (FComboSystemEditor::*SetCurrentSelectionEnumPtrType)(FString string_name);

	void ComboNode_DFS(UComboSystemGraphNode_Base *Node);

	void CompileComboSystem();

	static void SetupGraphNode(UComboTree *InComboSystem, UComboNode * InComboNode, bool bSelectNewNode);

	static void SetupRootGraphNode(UComboTree *InComboSystem, UComboNode * InComboNode, bool bSelectNewNode);

private:

	/** The ComboSystem asset being inspected */
	UComboTree* ComboTree;

	/** New Graph Editor */
	TSharedPtr<SGraphEditor> ComboSystemGraphEditor;

	/** Create a new graph canvas */
	TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);

	void SetEditorPropertiesSelectedNode(UObject * SelectedNode);

protected:

	/** Called when the selection changes in the GraphEditor */
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	/** New Graph Editor */
	//TSharedPtr<SGraphEditor> ComboSystemGraphEditor;

	/** Delete the currently selected nodes */
	void DeleteSelectedNodes();
	/** Whether we are able to delete the currently selected nodes */
	bool CanDeleteNodes();

	void CopySelectedNodes();

	bool CanCopyNodes();

	void CutSelectedNodes();

	bool CanCutNodes();

	void DeleteSelectedDuplicatableNodes();

	/** Called to undo the last action */
	void UndoGraphAction();

	/** Called to redo the last undone action */
	void RedoGraphAction();

public:

	void PasteNodes();

	void PasteNodesHere(const FVector2D & Location);
		
	bool CanPasteNodes() const;

	UComboTree* GetComboTree() const;

protected:

	void DuplicateNodes();
		
	bool CanDuplicateNodes();

	void SelectAllNodes();

	bool CanSelectAllNodes();

	void OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged);

	void AddComboTreeMenu(FMenuBarBuilder& MenuBuilder);

	void FillComboTreeMenu(FMenuBuilder& MenuBuilder);

	void FillComboTreeSubmenu(FMenuBuilder& MenuBuilder);

	void OnComboTreeBtnClicked();

	UFUNCTION()
	TSharedRef<SDockTab> SpawnComboTreeTab(const FSpawnTabArgs& TabSpawnArgs);

	void AddComboTreeToolbarExtension(class FToolBarBuilder& Builder);

	void AddTimingMenuEntry(FMenuBuilder& MenuBuilder);

	void OnTimingCheckBoxChanged(ECheckBoxState InState);

	static int ComboEditorID;

	//FTabId PropertyTabId;

	const FName ComboTreeTabName = FName(*FString::Printf(TEXT("Properties-%d"), ComboEditorID));

	bool OnRequestClose() override;

	static FComboSystemEditor *GetFocusedComboSystemEditor()
	{
		return FocusedComboSystemEditor;
	}

private:

	/** FNotifyHook interface */
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, UProperty* PropertyThatChanged) override;

	/** Create new graph editor widget */
	TSharedRef<SGraphEditor> CreateGraphEditorWidget();

	/** Properties tab */
	TSharedPtr<class IDetailsView> ComboSystemProperties;

	/**	The tab ids for all the tabs used */
	static const FName GraphCanvasTabId;
	static const FName PropertiesTabId;

	/** Creates all internal widgets for the tabs to point at */
	void CreateInternalWidgets();

	/** Command list for this editor */
	TSharedPtr<FUICommandList> GraphEditorCommands;

	/** Custom Properties and other fields must be static, so when they need to grab the appropriate ComboSystemEditor to read, it will be this one*/
	static FComboSystemEditor *FocusedComboSystemEditor;

};
