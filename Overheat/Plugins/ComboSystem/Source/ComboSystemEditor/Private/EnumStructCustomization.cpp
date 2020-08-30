/* 
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "EnumStructCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"

#define LOCTEXT_NAMESPACE "EnumStructCustomization"

FComboSystemEditor * FEnumStructCustomization::ComboSystemEditorPtr = NULL;

TSharedRef<IPropertyTypeCustomization> FEnumStructCustomization::MakeInstance()
{
	return MakeShareable(new FEnumStructCustomization());
}

TSharedRef<IPropertyTypeCustomization> FEnumStructCustomization::MakeInstance(FComboSystemEditor::SetCurrentSelectionEnumPtrType SetCurrentSelectionEnumPtr)
{
	FEnumStructCustomization *CustomStruct = new FEnumStructCustomization();
	return MakeShareable(CustomStruct);
}

TSharedRef<IPropertyTypeCustomization> FEnumStructCustomization::MakeInstance(FComboSystemEditor * CSEditorPtr)
{
	FEnumStructCustomization *Customization = new FEnumStructCustomization();
	Customization->SetComboSystemEditorPtr(CSEditorPtr);
	return MakeShareable(Customization);
}

void FEnumStructCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, FDetailWidgetRow & HeaderRow, IPropertyTypeCustomizationUtils & StructCustomizationUtils)
{
	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);
	FString SelectedEnum = FString("");

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		const TSharedRef< IPropertyHandle > ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
		UProperty *debug_p = ChildHandle->GetProperty();
		FString Temp = FString("");
		ChildHandle->GetValue(Temp);
		if (!Temp.IsEmpty())
		{
			//send current selected enum string to combobox as the default
			SelectedEnum = Temp;
		}

		if (ChildHandle->GetProperty()->GetName() == TEXT("ComboEnum"))
		{
			EnumPropertyHandle = ChildHandle;
		}
	}

	check(EnumPropertyHandle.IsValid());

	TSharedRef<SComboDropDown> ComboBox = SNew(SComboDropDown)
											.CurrentEnumStringArg(SelectedEnum);

	ComboBox.Get().SetComboSystemEditorPtr(ComboSystemEditorPtr);

	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			ComboBox
		];
}

void FEnumStructCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder & StructBuilder, IPropertyTypeCustomizationUtils & StructCustomizationUtils)
{
}

#undef LOCTEXT_NAMESPACE