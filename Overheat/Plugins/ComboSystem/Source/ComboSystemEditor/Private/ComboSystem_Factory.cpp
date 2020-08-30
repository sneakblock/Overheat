/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystem_Factory.h"
#include "AssetTypeActions_ComboSystem.h"
#include "AssetToolsModule.h"
#include "ComboTree.h"
#include "ComboSystemGraph.h"


UComboSystem_Factory::UComboSystem_Factory(const class FObjectInitializer &OBJ) : Super(OBJ) {
	SupportedClass = UComboTree::StaticClass();
	bEditAfterNew = true;
	bCreateNew = true;
}

UObject* UComboSystem_Factory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UComboTree *NewComboSystem = NewObject<UComboTree>(InParent, Class, Name, Flags | RF_Transactional, Context);
	UComboSystemGraph::CreateNewComboSystemGraph(NewComboSystem);

	return NewComboSystem;
}