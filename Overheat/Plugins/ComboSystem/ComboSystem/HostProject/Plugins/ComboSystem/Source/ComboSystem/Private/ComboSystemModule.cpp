/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystemModule.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FComboSystemModule, ComboSystem);

DEFINE_LOG_CATEGORY(ComboSystem)

#define LOCTEXT_NAMESPACE "ComboSystem"

void FComboSystemModule::StartupModule()
{
	UE_LOG(ComboSystem, Warning, TEXT("ComboSystem: Log Started"));

}

void FComboSystemModule::ShutdownModule()
{
	UE_LOG(ComboSystem, Warning, TEXT("ComboSystem: Log Ended"));
}

#undef LOCTEXT_NAMESPACE
