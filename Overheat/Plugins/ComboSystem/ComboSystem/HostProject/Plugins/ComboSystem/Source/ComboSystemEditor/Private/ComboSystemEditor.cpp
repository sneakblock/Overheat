/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

//set to 1 for extra output information
#define DEBUG 0

#include "ComboSystemEditor.h"
#include "HAL/PlatformApplicationMisc.h"
#include "AssetToolsModule.h"
#include "EditorStyleSet.h"
#include "Editor.h"
#include "SNodePanel.h"
#include "Slate.h"
#include "IDetailsView.h"
#include "GraphEditor.h"
#include "GraphEditorActions.h"
#include "ScopedTransaction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "PropertyEditorModule.h"
#include "ComboSystemGraphEditorCommands.h"
#include "Runtime/Slate/Public/Framework/Docking/TabManager.h"
#include "Runtime/Slate/Public/Widgets/Text/SInlineEditableTextBlock.h"
#include "EnumStructCustomization.h"
#include "FrameAndTimeCustomization.h"
#include "ComboNode.h"
#include "ComboSystemGraphNode_Root.h"
#include "ComboSystemGraphNode.h"
#include "EdGraphUtilities.h"
#include "Misc/CoreDelegates.h"


#define LOCTEXT_NAMESPACE "ComboSystemEditor"

const FName FComboSystemEditor::GraphCanvasTabId(TEXT("ComboSystemEditor_GraphCanvas"));
const FName FComboSystemEditor::PropertiesTabId(TEXT("ComboSystemEditor_Properties"));

FComboSystemEditor::FComboSystemEditor()
	: ComboTree(nullptr)
{
	ComboEditorID++; 
	TSharedRef<class FGlobalTabmanager> GlobalTabManager = FGlobalTabmanager::Get();
	TabForegroundedDelegateHandle = GlobalTabManager->OnTabForegrounded_Subscribe(FOnActiveTabChanged::FDelegate::CreateRaw(this, &FComboSystemEditor::OnTabForegrounded));
}

int FComboSystemEditor::ComboEditorID = 0;
FComboSystemEditor *FComboSystemEditor::FocusedComboSystemEditor = NULL;

FComboSystemEditor::~FComboSystemEditor()
{
	GEditor->UnregisterForUndo(this);
	TSharedRef<class FGlobalTabmanager> GlobalTabManager = FGlobalTabmanager::Get();
	GlobalTabManager->OnTabForegrounded_Unsubscribe(TabForegroundedDelegateHandle);

	UnregisterTabSpawners(GlobalTabManager);
}

void FComboSystemEditor::InitComboSystemEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject * ObjectToEdit)
{
	ComboTree = CastChecked<UComboTree>(ObjectToEdit);
	// Support undo/redo
	ComboTree->SetFlags(RF_Transactional);

	GEditor->RegisterForUndo(this);

	if(DEBUG) UE_LOG(LogTemp, Log, TEXT("ComboSystem Editor Initialized"));

	FGraphEditorCommands::Register();
	FComboSystemGraphEditorCommands::Register();

	BindGraphCommands();

	CreateInternalWidgets();

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_ComboSystemEditor_Layout_v3")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.20f)
					->AddTab(PropertiesTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.80f)
					->AddTab(GraphCanvasTabId, ETabState::OpenedTab)->SetHideTabWell(true)
				)
			)
		);

	// generate parent
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, TEXT("ComboSystemEditorApp"), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectToEdit, true);

	//add custom menu actions
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender);


	MenuExtender->AddToolBarExtension(
		"Settings",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateRaw(this, &FComboSystemEditor::AddComboTreeToolbarExtension));

	MenuExtender->AddMenuBarExtension(
		"Help",
		EExtensionHook::After,
		GetToolkitCommands(),
		FMenuBarExtensionDelegate::CreateRaw(this, &FComboSystemEditor::AddComboTreeMenu)
	);

	FAssetEditorToolkit::AddMenuExtender(MenuExtender);

	TSharedRef<class FGlobalTabmanager> GlobalTabManager = FGlobalTabmanager::Get();
	GlobalTabManager->RegisterNomadTabSpawner(ComboTreeTabName, FOnSpawnTab::CreateRaw(this, &FComboSystemEditor::SpawnComboTreeTab))
		.SetDisplayName(FText::FromString(TEXT("Combo Tree Properties")));

	FAssetEditorToolkit::RegenerateMenusAndToolbars();

}

void FComboSystemEditor::OnTabForegrounded(TSharedPtr<SDockTab> ForegroundedTab, TSharedPtr<SDockTab> BackgroundedTab) 
{
	if (ComboSystemGraphEditor.IsValid())
	{
		/*
			When we click over to another combo system editor while nodes are selected, those nodes are still selected for that editor.
			Unfortunatley, when we copy, paste, or do some other standard graph action, this applies to all editors that have currently 
			selected nodes, even if we aren't on that tab.
			To prevent this, we deselect the current selection whenever tabs are changed.
		*/
		ComboSystemGraphEditor->ClearSelectionSet();
	}
}

void FComboSystemEditor::FocusWindow(UObject * ObjectToFocusOn)
{
	FAssetEditorToolkit::FocusWindow(ObjectToFocusOn);
}

void FComboSystemEditor::InvokeTab(const FTabId & TabId)
{
	FAssetEditorToolkit::InvokeTab(TabId);
}

bool FComboSystemEditor::OnRequestClose()
{
	TSharedRef<class FGlobalTabmanager> tm = FGlobalTabmanager::Get();
	tm->UnregisterNomadTabSpawner(ComboTreeTabName);
	// UnRegister the things we made in CreateInternalWidgets()
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	ComboSystemProperties->UnregisterInstancedCustomPropertyTypeLayout("EnumStruct", nullptr);
	ComboSystemProperties->UnregisterInstancedCustomPropertyLayout(UComboNode::StaticClass());

	return true;
}

void FComboSystemEditor::AddComboTreeToolbarExtension(class FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(
		FExecuteAction::CreateRaw(this, &FComboSystemEditor::OnComboTreeBtnClicked));
}

void FComboSystemEditor::AddComboTreeMenu(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
		FText::FromString("Combo Tree"),
		FText::FromString("Adjust combo tree settings"),
		FNewMenuDelegate::CreateRaw(this, &FComboSystemEditor::AddTimingMenuEntry),
		"Custom"
	);
}

void FComboSystemEditor::OnComboTreeBtnClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(ComboTreeTabName);
}

TSharedRef<SDockTab> FComboSystemEditor::SpawnComboTreeTab(const FSpawnTabArgs& TabSpawnArgs)
{
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Combo Tree Properties Tab")))
		.ContentPadding(3)
		];

	return SpawnedTab;
}

void FComboSystemEditor::OnTimingCheckBoxChanged(ECheckBoxState InState)
{
	switch (InState)
	{
		case(ECheckBoxState::Checked):
			GetComboTree()->IsFrameIndependent = true;
			break;
		case(ECheckBoxState::Unchecked):
			GetComboTree()->IsFrameIndependent = false;
			break;
		default:
			if (DEBUG)  UE_LOG(LogTemp, Warning, TEXT("Check box changed to invalid state"));
	}

	ReselectNodes();
}

void FComboSystemEditor::AddTimingMenuEntry(FMenuBuilder& MenuBuilder)
{
	
	// Create Section
	MenuBuilder.BeginSection("CustomMenu", TAttribute<FText>(FText::FromString("Properties")));
	{
		MenuBuilder.AddWidget(SNew(SCheckBox)
			.OnCheckStateChanged(this, &FComboSystemEditor::OnTimingCheckBoxChanged)
			.IsChecked(GetComboTree()->IsFrameIndependent ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			, FText::FromString(TEXT("Frame Rate Independent")), false, true);

	}
	MenuBuilder.EndSection();
}


void FComboSystemEditor::AddReferencedObjects(FReferenceCollector & Collector)
{
}

FName FComboSystemEditor::GetToolkitFName() const
{
	return FName("ComboSystemEditor");
}

FText FComboSystemEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "ComboSystem Editor");
}

FString FComboSystemEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "ComboSystem ").ToString();
}

FLinearColor FComboSystemEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

void FComboSystemEditor::SaveAsset_Execute()
{
	CompileComboSystem();					
	FAssetEditorToolkit::SaveAsset_Execute();
}

void FComboSystemEditor::SaveAssetAs_Execute()
{
	CompileComboSystem();					
	FAssetEditorToolkit::SaveAssetAs_Execute();
}

void FComboSystemEditor::CompileComboSystem()
{
	UComboSystemGraphNode_Base * RootGraphNode = Cast<UComboSystemGraphNode_Base>(ComboTree->RootNode->GraphNode);
	ComboNode_DFS(RootGraphNode);
}

/*DFS of the graph nodes to connect the actual combo nodes*/
void FComboSystemEditor::ComboNode_DFS(UComboSystemGraphNode_Base *Node)
{
	Node->ComboNode->InitNode();

	TArray<UEdGraphPin *> OutputPins = Node->GetOutputPins();
	if (OutputPins.Num() > 0) //if this node has output pins
	{
		for (int32_t PinIndex = 0; PinIndex < OutputPins.Num(); PinIndex++) //for each output pin
		{
			UEdGraphPin *OutPin = OutputPins[PinIndex];
			TArray<UEdGraphPin *> LinkedToPins = OutPin->LinkedTo;
			if (LinkedToPins.Num() > 0)	//if this output pin is connected to at least one corresponding input pin
			{
				for (UEdGraphPin * LinkedToPin : LinkedToPins)
				{
					UComboSystemGraphNode *ChildGraphNode = Cast<UComboSystemGraphNode>(LinkedToPin->GetOwningNode()); // root nodes should never have an input
					UComboNode *ChildNode = ChildGraphNode->ComboNode;
					// setup NodePosY and any other information a ComboNode needs to extract from its ComboGraphNode
					ChildNode->NodePosY = ChildGraphNode->NodePosY;
					Node->ComboNode->SetChild(ChildNode, PinIndex);
					ComboNode_DFS(ChildGraphNode);
					Node->ComboNode->Reorder();
				}
			}
		}
	}
}

void FComboSystemEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	if (DEBUG)  UE_LOG(LogTemp, Warning, TEXT("tabs Registered!"));

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ComboSystemEditor", "Combo System Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(GraphCanvasTabId, FOnSpawnTab::CreateSP(this, &FComboSystemEditor::SpawnTab_GraphCanvas))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "ComboGraph"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FComboSystemEditor::SpawnTab_Properties))
		.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

}

void FComboSystemEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager); 

	InTabManager->UnregisterTabSpawner(GraphCanvasTabId);
	InTabManager->UnregisterTabSpawner(PropertiesTabId);

}

TSet<UObject*> FComboSystemEditor::GetSelectedNodes_WithoutRoot()
{
	FGraphPanelSelectionSet CurrentSelection;
	if (ComboSystemGraphEditor.IsValid())
	{
		CurrentSelection = ComboSystemGraphEditor->GetSelectedNodes();
	}
	for (UObject *SelectedObject : CurrentSelection)
	{
		if (Cast<UComboSystemGraphNode_Root>(SelectedObject))
		{
			CurrentSelection.Remove(SelectedObject);
			break;
		}
	}
	return CurrentSelection;
}


TSet<UObject*> FComboSystemEditor::GetSelectedNodes()
{
	FGraphPanelSelectionSet CurrentSelection;
	if (ComboSystemGraphEditor.IsValid())
	{
		CurrentSelection = ComboSystemGraphEditor->GetSelectedNodes();
	}
	return CurrentSelection;
}

void FComboSystemEditor::SetSelection(TArray<UObject*> SelectedObjects)
{
	if (ComboSystemProperties.IsValid())
	{
		ComboSystemProperties->SetObjects(SelectedObjects);
	}
}

void FComboSystemEditor::ReselectNodes()
{
	// unselect everything so user has to refresh display view
	auto SelectedNodes = GetSelectedNodes();
	SetSelection(TArray<UObject *>());
	SetSelection(SelectedNodes.Array());
	// Update UI
	ComboSystemGraphEditor->NotifyGraphChanged();
	ComboTree->PostEditChange();
	ComboTree->MarkPackageDirty();
}

void FComboSystemEditor::BindGraphCommands()
{
	const FComboSystemGraphEditorCommands& Commands = FComboSystemGraphEditorCommands::Get();

	ToolkitCommands->MapAction(
		Commands.SetCombo,
		FExecuteAction::CreateSP(this, &FComboSystemEditor::SetCombo));

	ToolkitCommands->MapAction(
		FGenericCommands::Get().Undo,
		FExecuteAction::CreateSP(this, &FComboSystemEditor::UndoGraphAction));

	ToolkitCommands->MapAction(
		FGenericCommands::Get().Redo,
		FExecuteAction::CreateSP(this, &FComboSystemEditor::RedoGraphAction));
}

void FComboSystemEditor::SetCombo()
{
	if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("SetCombo called!"));
}

void FComboSystemEditor::SetCurrentSelectionEnum(FString EnumName)
{
	if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("SetCurrentSelectionEnum called!"));

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UComboSystemGraphNode* Node = Cast<UComboSystemGraphNode>(*SelectedIter))
		{
			Node->UpdateComboNodeEnum(EnumName);
		}
		else if (UComboSystemGraphNode_Root *RootNode = Cast<UComboSystemGraphNode_Root>(*SelectedIter))
		{
			RootNode->UpdateComboNodeEnum(EnumName);
		}
	}


	//TSet<UObject*> CurrentNodes = GetSelectedNodes();
	//TArray<UObject *> CurrentNodeArray = CurrentNodes.Array();
	//for (UObject *NodePtr : CurrentNodeArray)
	//{
	//	UComboSystemGraphNode *Node = Cast<UComboSystemGraphNode>(NodePtr);
	//	if (Node == nullptr) {
	//		UComboSystemGraphNode_Root *RootNode = Cast<UComboSystemGraphNode_Root>(NodePtr);
	//		RootNode->UpdateComboNodeEnum(EnumName);
	//	}
	//	else {
	//		Node->UpdateComboNodeEnum(EnumName);
	//	}
	//	
	//	if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("test called!"));
	//}
}


TSharedRef<SDockTab> FComboSystemEditor::SpawnTab_GraphCanvas(const FSpawnTabArgs & Args)
{
	if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("graph canvas tab!"));

	check(Args.GetTabId() == GraphCanvasTabId);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("ComboSystemGraphCanvasTitle", "ComboGraph"));

	if (ComboSystemGraphEditor.IsValid())
	{
		SpawnedTab->SetContent(ComboSystemGraphEditor.ToSharedRef());
	}

	return SpawnedTab;
}

// When spawning the properties tab, it is recorded so that we can compare it in the OnForegrounded method
TSharedRef<SDockTab> FComboSystemEditor::SpawnTab_Properties(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == PropertiesTabId);



	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("ComboSystemDetailsTitle", "Combo Details"))
		[
			ComboSystemProperties.ToSharedRef()
		];
}

void FComboSystemEditor::SetEditorPropertiesSelectedNode(UObject* SelectedNode)
{
	ComboSystemProperties->SetObject(SelectedNode);
}

void FComboSystemEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	if (NewSelection.Num())
	{
		for (TSet<class UObject*>::TConstIterator SetIt(NewSelection); SetIt; ++SetIt)
		{
			if (UComboSystemGraphNode* GraphNode = Cast<UComboSystemGraphNode>(*SetIt))
			{
				Selection.Add(GraphNode->ComboNode);
			}
			else
			{
				Selection.Add(*SetIt);
			}
		}

	}
	else
	{
		//select the Combo Tree object itself
		Selection.Add(GetComboTree());
	}

	SetSelection(Selection);
}

void FComboSystemEditor::DeleteSelectedNodes()
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "ComboSystemEditorDeleteSelectedNode", "Delete Selected Combo System Node"));

	ComboSystemGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes_WithoutRoot();

	ComboSystemGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);

		if (Node->CanUserDeleteNode())
		{
			if (UComboSystemGraphNode* ComboGraphNode = Cast<UComboSystemGraphNode>(Node))
			{
				UComboNode* DelNode = ComboGraphNode->ComboNode;
				UComboNode* DelSnapshotNode = ComboGraphNode->ComboNodeSnapshot;
				UComboNode* Parent = DelNode->ParentNode;

				FBlueprintEditorUtils::RemoveNode(NULL, ComboGraphNode, true); //call BreakNodeLinks() in graph schema

				ComboTree->AllNodes.Remove(DelNode);
				ComboTree->AllNodes.Remove(DelSnapshotNode);

				ComboTree->MarkPackageDirty();
			}
			else
			{
				FBlueprintEditorUtils::RemoveNode(NULL, Node, true);
			}
		}
	}
}

bool FComboSystemEditor::CanDeleteNodes()
{
	return CanCopyNodes();
}

void FComboSystemEditor::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes_WithoutRoot();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UComboSystemGraphNode* Node = Cast<UComboSystemGraphNode>(*SelectedIter))
		{
			Node->TakeSnapshot();
			Node->PrepareForCopying();
		}
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, /*out*/ ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);

	// Make sure Combo remains the owner of the copied nodes
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UComboSystemGraphNode* Node = Cast<UComboSystemGraphNode>(*SelectedIter))
		{
			Node->PostCopyNode();
		}
	}
}

bool FComboSystemEditor::CanCopyNodes()
{
	return true;
}

void FComboSystemEditor::PasteNodes()
{
	PasteNodesHere(ComboSystemGraphEditor->GetPasteLocation());
}

void FComboSystemEditor::PasteNodesHere(const FVector2D& Location)
{
	// Undo/Redo support
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "ComboSystemEditorPaste", "Paste Combo System Node"));
	ComboTree->GetGraph()->Modify();
	ComboTree->Modify();

	// Clear the selection set (newly pasted stuff will be selected)
	ComboSystemGraphEditor->ClearSelectionSet();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(ComboTree->GetGraph(), TextToImport, /*out*/ PastedNodes);

	//Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f, 0.0f);

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}

	if (PastedNodes.Num() > 0)
	{
		float InvNumNodes = 1.0f / float(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;

		if (UComboSystemGraphNode* ComboSystemGraphNode = Cast<UComboSystemGraphNode>(Node))
		{
			ComboSystemGraphNode->ComboNode = ComboTree->ConstructComboNode<UComboNode>();
			ComboSystemGraphNode->ComboNode->CopyState(ComboSystemGraphNode->ComboNodeSnapshot);
			ComboSystemGraphNode->ComboNode->GraphNode = ComboSystemGraphNode;
			ComboSystemGraphNode->InitializeSnapshot(ComboTree);

			ComboSystemGraphNode->UpdateComboNodeEnum(ComboSystemGraphNode->ComboNode->ComboStructEnum.ComboEnumString);

		}

		// Select the newly pasted stuff
		ComboSystemGraphEditor->SetNodeSelection(Node, true);

		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

		Node->SnapToGrid(SNodePanel::GetSnapGridSize());

		// Give new node a different Guid from the old one
		Node->CreateNewGuid();
	}

	// Update UI
	ComboSystemGraphEditor->NotifyGraphChanged();
	ComboTree->PostEditChange();
	ComboTree->MarkPackageDirty();
}

bool FComboSystemEditor::CanPasteNodes() const
{
	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(ComboTree->GetGraph(), ClipboardContent);
}

void FComboSystemEditor::CreateInternalWidgets()
{
	ComboSystemGraphEditor = CreateGraphEditorWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	/*
		RegisterCustomPropertyTypeLayout Bug

		There was a bug that was introduced by the following line: 
			PropertyModule.RegisterCustomPropertyTypeLayout("EnumStruct", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEnumStructCustomization::MakeInstance, this));
		The problem is that this overload of CustomPropertyTypeLayout() does have a reference to the DetailView, and silently registers it statically.
		The next time CustomPropertyTypeLayout() is called in this way, the function is overwritten, so each time the ComboSystemEditor is created it 
		would overwrite the FEnumStructCustomization for all Editor instances with the one for this Editor. What this looked like is that anytime 
		you tried to set the EnumStruct of a ComboNode from any Editor besides the last one that was created, it would fail because it would try 
		to access the last opened Editor.
		
		It was fixed by changing the line to:
			PropertyModule.RegisterCustomPropertyTypeLayout("EnumStruct", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEnumStructCustomization::MakeInstance, this), nullptr, DetailView);
		This overload provides the DetailView, which internally lets the RegisterCustomPropertyTypeLayout() function set an INSTANCED version, which 
		means the property customization is registered to this Editor, not statically for all ComboSystem Editors.

		EDIT: Epic decided to deprecate the instanced overload of RegisterCustomPropertyTypeLayout() so now you have to do it in the way as shown below.

		To do instanced DetailCustomization you have to use a similar tactic. If you don't need state, then the following line can be used:
			PropertyModule.RegisterCustomClassLayout("ComboNode", FOnGetDetailCustomizationInstance::CreateStatic(&FrameAndTimeCustomization::MakeInstance, this));
		This has the same problem of above. If you create another Editor and this code gets run again, it is static so it will overwrite the ComboNode class customization.
		To do instanced, create an FOnGetDetailCustomizationInstance and use RegisterInstancedCustomPropertyLayout as shown below.
	*/

	//Custom detail views

	TSharedRef<IDetailsView> DetailView = PropertyModule.CreateDetailView(Args);
	// overriding EnumStruct property to show a ComboBox of valid Enums (INSTANCED)
	DetailView->RegisterInstancedCustomPropertyTypeLayout("EnumStruct", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEnumStructCustomization::MakeInstance, this));
	// overriding combo node details so it only shows time for frame-independent and only shows frames for frame-dependent (INSTANCED)
	FOnGetDetailCustomizationInstance CustomizeFrameAndTimeForEditor = FOnGetDetailCustomizationInstance::CreateStatic(&FrameAndTimeCustomization::MakeInstance, this);
	DetailView->RegisterInstancedCustomPropertyLayout(UComboNode::StaticClass(), CustomizeFrameAndTimeForEditor);
	ComboSystemProperties = DetailView;
}

void FComboSystemEditor::CutSelectedNodes()
{
	CopySelectedNodes();
	// Cut should only delete nodes that can be duplicated
	DeleteSelectedDuplicatableNodes();
}

bool FComboSystemEditor::CanCutNodes()
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FComboSystemEditor::DeleteSelectedDuplicatableNodes()
{
	// Cache off the old selection
	const FGraphPanelSelectionSet OldSelectedNodes = GetSelectedNodes_WithoutRoot();

	// Clear the selection and only select the nodes that can be duplicated
	FGraphPanelSelectionSet RemainingNodes;
	ComboSystemGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if ((Node != NULL) && Node->CanDuplicateNode())
		{
			ComboSystemGraphEditor->SetNodeSelection(Node, true);
		}
		else
		{
			RemainingNodes.Add(Node);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	// Reselect whatever's left from the original selection after the deletion
	ComboSystemGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(RemainingNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			ComboSystemGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FComboSystemEditor::DuplicateNodes()
{
	// Copy and paste current selection
	CopySelectedNodes();
	PasteNodes();
}

bool FComboSystemEditor::CanDuplicateNodes()
{
	return CanCopyNodes();
}

void FComboSystemEditor::SelectAllNodes()
{
	ComboSystemGraphEditor->SelectAllNodes();
}

bool FComboSystemEditor::CanSelectAllNodes()
{
	return true;
}


TSharedRef<SGraphEditor> FComboSystemEditor::CreateGraphEditorWidget()
{
	if (!GraphEditorCommands.IsValid())
	{

		if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("Creating Graph Editor Widget!"));
		GraphEditorCommands = MakeShareable(new FUICommandList);

		// Editing commands
		GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
			FExecuteAction::CreateSP(this, &FComboSystemEditor::SelectAllNodes),
			FCanExecuteAction::CreateSP(this, &FComboSystemEditor::CanSelectAllNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FComboSystemEditor::DeleteSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FComboSystemEditor::CanDeleteNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
			FExecuteAction::CreateSP(this, &FComboSystemEditor::CopySelectedNodes),
			FCanExecuteAction::CreateSP(this, &FComboSystemEditor::CanCopyNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
			FExecuteAction::CreateSP(this, &FComboSystemEditor::CutSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FComboSystemEditor::CanCutNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
			FExecuteAction::CreateSP(this, &FComboSystemEditor::PasteNodes),
			FCanExecuteAction::CreateSP(this, &FComboSystemEditor::CanPasteNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateSP(this, &FComboSystemEditor::DuplicateNodes),
			FCanExecuteAction::CreateSP(this, &FComboSystemEditor::CanDuplicateNodes)
		);
	}

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_ComboSystem", "COMBO TREE");

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FComboSystemEditor::OnSelectedNodesChanged);
	InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FComboSystemEditor::OnNodeTitleCommitted);
	//InEvents.OnNodeDoubleClicked = 

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(ComboTree->GetGraph()) // this is where editor gets graph
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FComboSystemEditor::OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged)
{
	if (NodeBeingChanged)
	{
		const FScopedTransaction Transaction(LOCTEXT("RenameNode", "Rename Node"));
		NodeBeingChanged->Modify();
		NodeBeingChanged->OnRenameNode(NewText.ToString());
	}
}

UComboTree * FComboSystemEditor::GetComboTree() const
{
	return ComboTree;
}


void FComboSystemEditor::NotifyPostChange(const FPropertyChangedEvent & PropertyChangedEvent, UProperty * PropertyThatChanged)
{
	if (ComboSystemGraphEditor.IsValid() && PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
	{
		ComboSystemGraphEditor->NotifyGraphChanged();
	}


}

void FComboSystemEditor::PostUndo(bool bSuccess)
{
	if (ComboSystemGraphEditor.IsValid())
	{
		ComboSystemGraphEditor->ClearSelectionSet();
		ComboSystemGraphEditor->NotifyGraphChanged();
	}

}

void FComboSystemEditor::UndoGraphAction()
{
	GEditor->UndoTransaction();
}

void FComboSystemEditor::RedoGraphAction()
{
	// Clear selection, to avoid holding refs to nodes that go away
	ComboSystemGraphEditor->ClearSelectionSet();

	GEditor->RedoTransaction();
}

void FComboSystemEditor::SetupGraphNode(UComboTree *InComboSystem, UComboNode * InComboNode, bool bSelectNewNode)
{
	// Create the graph node
	check(InComboNode->GraphNode == NULL);
	UEnum *ComboEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EComboDefault"), true);//where combo is set the first time
	FGraphNodeCreator<UComboSystemGraphNode> NodeCreator(*(InComboSystem->GetGraph()));
	UComboSystemGraphNode* GraphNode = NodeCreator.CreateNode(bSelectNewNode);
	GraphNode->SetComboNode(InComboNode, ComboEnumPtr);
	GraphNode->InitializeSnapshot(InComboSystem);
	NodeCreator.Finalize();
}

void FComboSystemEditor::SetupRootGraphNode(UComboTree *InComboSystem, UComboNode * InComboNode, bool bSelectNewNode = true)
{
	// Create the root graph node
	check(InComboNode->GraphNode == NULL);
	UEnum *ComboEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EComboDefault"), true);//where combo is set the first time
	FGraphNodeCreator<UComboSystemGraphNode_Root> NodeCreator(*(InComboSystem->GetGraph()));
	UComboSystemGraphNode_Root* GraphNode = NodeCreator.CreateNode(bSelectNewNode);
	const int32 RootNodeHeightOffset = -58;

	// Create the result node
	GraphNode->NodePosY = RootNodeHeightOffset;
	InComboSystem->GetGraph()->GetSchema()->SetNodeMetaData(GraphNode, FNodeMetadata::DefaultGraphNode);
	GraphNode->SetComboNode(InComboNode, ComboEnumPtr);
	GraphNode->InitializeSnapshot(InComboSystem);
	NodeCreator.Finalize();

}