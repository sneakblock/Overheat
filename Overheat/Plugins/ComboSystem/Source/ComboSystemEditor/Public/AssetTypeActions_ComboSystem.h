/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions/AssetTypeActions_ClassTypeBase.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

class COMBOSYSTEMEDITOR_API FAssetTypeActions_ComboSystem : public FAssetTypeActions_Base
{
public:
	//// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ComboSystem", "ComboTree"); }
	virtual FColor GetTypeColor() const override { return FColor(97, 85, 212); }
	virtual UClass* GetSupportedClass() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
	//virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	//virtual void AssetsActivated(const TArray<UObject*>& InObjects, EAssetTypeActivationMethod::Type ActivationType) override;
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }
	//virtual bool CanFilter() override { return false; }
	virtual FText GetAssetDescription(const FAssetData &AssetData) const override { return LOCTEXT("ComboSystemDesc", "ComboTree asset holding a graph of nodes"); }

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;


};
