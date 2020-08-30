/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECombo_MovementMode : uint8 {
	AIR				UMETA(DisplayName = "In Air"),
	GROUND			UMETA(DisplayName = "On Ground"),
	CROUCH			UMETA(DisplayName = "Crouched"),

	ALL				UMETA(DisplayName = "All") //ALL must be last enum in the list
};
