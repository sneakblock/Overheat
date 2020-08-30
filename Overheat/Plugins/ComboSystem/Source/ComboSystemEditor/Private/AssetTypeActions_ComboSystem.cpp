/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "AssetTypeActions_ComboSystem.h"
#include "ComboTree.h"
#include "ComboSystemEditor.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass * FAssetTypeActions_ComboSystem::GetSupportedClass() const
{
	return UComboTree::StaticClass();
}

void FAssetTypeActions_ComboSystem::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto ComboSystem = Cast<UComboTree>(*ObjIt);
		if (ComboSystem != NULL)
		{
			TSharedRef<FComboSystemEditor> NewComboSystemEditor(new FComboSystemEditor());
			NewComboSystemEditor->InitComboSystemEditor(Mode, EditWithinLevelEditor, ComboSystem);

		}
	}
}
