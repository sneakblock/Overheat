/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetTypeCategories.h"
#include "UnrealEd.h"
#include "ComboSystem_Factory.generated.h"

UCLASS()
class COMBOSYSTEMEDITOR_API UComboSystem_Factory : public UFactory
{
	GENERATED_UCLASS_BODY()

protected:
	virtual bool IsMacroFactory() const { return false; }
public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};