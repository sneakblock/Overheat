/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "IPropertyTypeCustomization.h"
#include "Slate.h"
#include "Engine.h"
#include "PropertyHandle.h"
#include "ComboSystemEditor.h"

#pragma once

class FEnumStructCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	static TSharedRef<IPropertyTypeCustomization> MakeInstance(FComboSystemEditor::SetCurrentSelectionEnumPtrType CurrentSelectionEnumPtr);

	static TSharedRef<IPropertyTypeCustomization> MakeInstance(FComboSystemEditor *CSEditorPtr);

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	FComboSystemEditor::SetCurrentSelectionEnumPtrType CurrentSelectionEnumPtr;
	
	static void SetComboSystemEditorPtr(FComboSystemEditor *CSEditorPtr)
	{
		ComboSystemEditorPtr = CSEditorPtr;
	}

	static FComboSystemEditor *ComboSystemEditorPtr;

private:
	TSharedPtr<IPropertyHandle> EnumPropertyHandle;
};

#define LOCTEXT_NAMESPACE "SComboDropDown"

class SComboDropDown : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SComboDropDown)
	{}
	SLATE_ARGUMENT(FString, CurrentEnumStringArg);
	SLATE_END_ARGS()

	TWeakObjectPtr<class FString> CurrentEnumString;		
	typedef TSharedPtr<FString> FComboItemType;

	//create the combo dropdown - given a default UEnum string
	void Construct(const FArguments& InArgs)
	{
		TSharedPtr<FString> Default = MakeShareable(new FString(InArgs._CurrentEnumStringArg));

		if (Default->IsEmpty())
		{
			Default = MakeShareable(new FString("EComboDefault"));
		}
		Options.Add(Default); 


		for (TObjectIterator<UEnum> Itr; Itr; ++Itr)
		{
			if (Itr->HasMetaData(TEXT("Category")))
			{
				const FString& Category = Itr->GetMetaData(TEXT("Category"));
				if (Category.Equals("Combo"))
				{
					FText name = FText::FromName(Itr->GetFName());
					FString EnumString = FString(name.ToString());
					if (Default.Get()->Compare(EnumString) != 0)
					{
						Options.Add(MakeShareable(new FString(name.ToString())));
					}
				}
			}
		}
		
		CurrentItem = Options[0];

		ChildSlot
			[
				SNew(SComboBox<FComboItemType>)
				.OptionsSource(&Options)
				.OnSelectionChanged(this, &SComboDropDown::OnSelectionChanged)
				.OnGenerateWidget(this, &SComboDropDown::MakeWidgetForOption)
				.InitiallySelectedItem(CurrentItem)
			[
				SNew(STextBlock)
				.Text(this, &SComboDropDown::GetCurrentItemLabel)
			]
			];
	}

	TSharedRef<SWidget> MakeWidgetForOption(FComboItemType InOption)
	{
		return SNew(STextBlock).Text(FText::FromString(*InOption));
	}

	void OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type)
	{
		CurrentItem = NewValue; // called whenever there is a change
		FString currentName = CurrentItem->Append("");

		if(ComboSystemEditorPtr)
			ComboSystemEditorPtr->SetCurrentSelectionEnum(currentName);
	}

	FText GetCurrentItemLabel() const
	{
		if (CurrentItem.IsValid())
		{
			return FText::FromString(*CurrentItem);
		}

		return LOCTEXT("InvalidComboEntryText", "<<Invalid option>>");
	}

	void SetComboSystemEditorPtr(FComboSystemEditor *CSEditorPtr)
	{
		ComboSystemEditorPtr = CSEditorPtr;
	}

	FComboSystemEditor *ComboSystemEditorPtr;

	FComboItemType CurrentItem;

	TArray<FComboItemType> Options;

	TSharedPtr<IPropertyHandle> EnumProperty;
};

#undef LOCTEXT_NAMESPACE