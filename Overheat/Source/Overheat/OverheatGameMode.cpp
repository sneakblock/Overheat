// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OverheatGameMode.h"
#include "OverheatCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOverheatGameMode::AOverheatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/PlayerBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
