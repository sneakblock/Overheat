/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#include "ComboSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogComboSystem);

UComboSystem::UComboSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	PrimaryComponentTick.bCanEverTick = false;
	ComboSystemUniqueName = MakeUniqueObjectName(GetOwner(), UComboSystem::StaticClass());
}

/***External****/

bool UComboSystem::AdvanceToNextCombo(uint8 EnumValue)
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return false;
	}
	if (!CurrentComboNode || !RootNode)
	{
		PrintDebugMessage("AdvanceToNextCombo called before ComboSystem initialization", FColor::Red);
		InitComboSystem();
	}

	ComboMutex.Lock();
	if (IsNextComboAvailable())
	{
		// only show nodes that we are in mode for
		TArray<UComboNode *>Children = CurrentComboNode->Combo_MovementModeMap[CurrentMode].ChildNodes; 

 		if (Children.Num() <= 0)
		{
			Children = CurrentComboNode->Combo_MovementModeMap[ECombo_MovementMode::ALL].ChildNodes;
		}

		for (UComboNode *Child : Children)
		{
			if (Child->InputEnumValue == EnumValue)
			{
				// should be ordered by y height; take first match
				if (ComboTree->IsFrameIndependent)
				{
					SetNextCombo_frameRateIndependent(Child);
				}
				else
				{
					SetNextCombo_frameRateDependent(Child);
				}
				ComboMutex.Unlock();
				PrintDebugMessage("Advanced to combo: " + Child->ComboName, FColor::Cyan);
				return true;
			}
		}
		//if we are here there were none in mode
		if (CurrentMode != ECombo_MovementMode::ALL)
		{
			//check the all
			Children = CurrentComboNode->Combo_MovementModeMap[ECombo_MovementMode::ALL].ChildNodes;
			for (UComboNode *Child : Children)
			{
				if (Child->InputEnumValue == EnumValue)
				{
					// should be ordered by y height; take first match
					if (ComboTree->IsFrameIndependent)
					{
						SetNextCombo_frameRateIndependent(Child);
					}
					else
					{
						SetNextCombo_frameRateDependent(Child);
					}
					ComboMutex.Unlock();
					PrintDebugMessage("Advanced to combo: " + Child->ComboName, FColor::Cyan);
					return true;
				}
			}
		}
	}
	else
	{
		PrintDebugMessage("Cannot advance combo system right now!", FColor::Red);
	}

	ComboMutex.Unlock();
	return false;
}

bool UComboSystem::IsNextComboAvailable()
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return false;
	}
	if (!CurrentComboNode || !RootNode)
	{
		PrintDebugMessage("IsNextComboAvailable called before ComboSystem initialization", FColor::Red);
		InitComboSystem();
	}

	if (CurrentState == EComboState::STARTUP || CurrentState == EComboState::COOLDOWN)
	{
		return false;
	}
	if (CurrentComboNode->LandHitRequired && !HasHitLanded)
	{
		return false;
	}

	return true;
}

FString UComboSystem::GetCurrentComboName()
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return FString("");
	}
	if (!CurrentComboNode || !RootNode)
	{
		PrintDebugMessage("GetCurrentComboName called before ComboSystem initialization", FColor::Red);
		InitComboSystem();
	}

	return CurrentComboNode->ComboName;
}

FVector UComboSystem::GetCurrentComboLaunch()
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return FVector(0, 0, 0);
	}
	if (!CurrentComboNode || !RootNode)
	{
		PrintDebugMessage("GetCurrentComboLaunch called before ComboSystem initialization", FColor::Red);
		InitComboSystem();
	}

	return CurrentComboNode->LaunchForce;
}

FVector UComboSystem::GetCurrentComboEnemyLaunch()
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return FVector(0, 0, 0);
	}
	if (!CurrentComboNode || !RootNode)
	{
		PrintDebugMessage("GetCurrentComboLaunch called before ComboSystem initialization", FColor::Red);
		InitComboSystem();
	}

	return CurrentComboNode->EnemyLaunchForce;
}

float UComboSystem::GetCurrentComboMultiplier()
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return 1.0;
	}
	if (!CurrentComboNode || !RootNode)
	{
		PrintDebugMessage("GetCurrentComboMultiplier called before ComboSystem initialization", FColor::Red);
		InitComboSystem();
	}

	return CurrentComboNode->ComboMultiplier;
}

bool UComboSystem::SetMovementMode(uint8 EnumValue)
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return false;
	}
	if (!CurrentComboNode || !RootNode)
	{
		PrintDebugMessage("SetMovementMode called before ComboSystem initialization", FColor::Red);
		InitComboSystem();
	}

	ComboMutex.Lock();
	if (EnumValue >= 0 && EnumValue <= (uint8)ECombo_MovementMode::ALL)
	{
		CurrentMode = (ECombo_MovementMode)EnumValue;
		ComboMutex.Unlock();
		PrintDebugMessage("Movement Mode successfully set", FColor::Blue);
		return true;
	}
	ComboMutex.Unlock();
	PrintDebugMessage("The requested Movement Mode is unrecognized", FColor::Red);
	return false;
}

FString UComboSystem::Reset()
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return FString("");
	}

	FString InterruptedCombo = FString("");
	ComboMutex.Lock();

	GetWorld()->GetTimerManager().ClearTimer(FrameRateIndependentModeTimer);
	GetWorld()->GetTimerManager().ClearTimer(FrameRateDependentModeTimer);
	InterruptedCombo = GetCurrentComboName();
	CurrentComboNode = RootNode;
	CurrentState = EComboState::ROOT;
	HasHitLanded = false;

	ComboMutex.Unlock();
	PrintDebugMessage("ComboSystem was Reset", FColor::Purple);
	return InterruptedCombo;
}

void UComboSystem::LandHit()
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree detected", FColor::Red);
		return;
	}

	ComboMutex.Lock();
	HasHitLanded = true;
	ComboMutex.Unlock();
	PrintDebugMessage("HitLanded registered", FColor::Blue);
}

// Called when the game starts
void UComboSystem::BeginPlay()
{
	InitComboSystem();
	Super::BeginPlay();
}

// Called every frame if framerate-independent mode is off
void UComboSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!ComboTree)
	{
		PrintDebugMessage("No ComboTree, unable to Tick", FColor::Red);
		return;
	}
	if (!CurrentComboNode || !RootNode)
	{
		PrintDebugMessage("Tick called before ComboSystem initialization", FColor::Red);
		InitComboSystem();
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


/***INTERNAL***/

void UComboSystem::SetNextCombo_frameRateIndependent(UComboNode *NextComboNode)
{
	HasHitLanded = false;

	switch (CurrentState)
	{
		case(EComboState::ROOT):
		case(EComboState::INPUT):
			GetWorld()->GetTimerManager().ClearTimer(FrameRateIndependentModeTimer);
			CurrentComboNode = NextComboNode;
			GetWorld()->GetTimerManager().SetTimer(FrameRateIndependentModeTimer, this, &UComboSystem::TimerHandle_frameRateIndependent, CurrentComboNode->StartupTime, false);
			PrintDebugMessage(ComboStateNames[CurrentState] + " to STARTUP", FColor::Yellow);
			CurrentState = EComboState::STARTUP;
			if (CurrentComboNode->StartupTime <= 0)
			{
				//if startup time was 0, timer handle doesn't run. instead, run it now
				TimerHandle_frameRateIndependent();
			}

			break;
		case(EComboState::STARTUP):
		case(EComboState::COOLDOWN):
			//cannot change combos during startup or cooldown
			PrintDebugMessage("Cannot change combo during " + ComboStateNames[CurrentState], FColor::Red);
			break;
		default:
			//did not know what state was just requested
			PrintDebugMessage("Unknown state requested", FColor::Red);
			break;
	}
}

// timer handle for frame rate independent - fires when it's time to change states
void UComboSystem::TimerHandle_frameRateIndependent()
{
	GetWorld()->GetTimerManager().ClearTimer(FrameRateIndependentModeTimer);
	switch (CurrentState)
	{
		case(EComboState::ROOT):
		default:
			// we are waiting for input
			PrintDebugMessage("ROOT or unknown state unexpectedly reached", FColor::Red);
			break;
		case(EComboState::INPUT):
			// we need to change to a new state, and our current state is INPUT, so the user must have missed their INPUT time.

			// transition to cooldown
			CurrentState = EComboState::COOLDOWN;
			GetWorld()->GetTimerManager().SetTimer(FrameRateIndependentModeTimer, this, &UComboSystem::TimerHandle_frameRateIndependent, CurrentComboNode->CooldownTime, false);
			PrintDebugMessage("INPUT to COOLDOWN", FColor::Yellow);
			if (CurrentComboNode->CooldownTime <= 0)
			{
				//if cooldown time was 0, timer handle doesn't run. instead, run it now
				TimerHandle_frameRateIndependent();
			}
			break;
		case(EComboState::STARTUP):

			// transition to input
			CurrentState = EComboState::INPUT;
			GetWorld()->GetTimerManager().SetTimer(FrameRateIndependentModeTimer, this, &UComboSystem::TimerHandle_frameRateIndependent, CurrentComboNode->InputTime, false);
			PrintDebugMessage("STARTUP to INPUT", FColor::Yellow);
			if (CurrentComboNode->InputTime <= 0)
			{
				//if input time was 0, timer handle doesn't run. instead, run it now
				TimerHandle_frameRateIndependent();
			}

			break;
		case(EComboState::COOLDOWN):
			//return to root
			CurrentState = EComboState::ROOT;
			CurrentComboNode = RootNode;
			PrintDebugMessage("COOLDOWN to ROOT", FColor::Yellow);
			break;
	}
}

void UComboSystem::SetNextCombo_frameRateDependent(UComboNode *NextComboNode) 
{
	switch (CurrentState)
	{
	case(EComboState::ROOT):
	case(EComboState::INPUT):
		HasHitLanded = false;
		CurrentComboNode = NextComboNode;
		if (NextComboNode->StartupFrames > 0)
		{
			//go to next combo's startup phase
			FrameCountdown = NextComboNode->StartupFrames;
			PrintDebugMessage(ComboStateNames[CurrentState] + " to STARTUP", FColor::Yellow);
			CurrentState = EComboState::STARTUP;
		}
		else
		{ 
			//no startup frames, just go straight into next input phase
			FrameCountdown = NextComboNode->InputFrames;
			PrintDebugMessage(ComboStateNames[CurrentState] + " to INPUT", FColor::Yellow);
			CurrentState = EComboState::INPUT;
		}
		GetWorld()->GetTimerManager().SetTimer(FrameRateDependentModeTimer, this, &UComboSystem::TimerHandle_frameRateDependent, GetWorld()->GetDeltaSeconds(), false);
		break;
	case(EComboState::STARTUP):
	case(EComboState::COOLDOWN):
		//can't call setcombo if in startup or cooldown
		PrintDebugMessage("Cannot advance combos during " + ComboStateNames[CurrentState], FColor::Red);
		break;
	default:
		//did not know what state was just requested
		PrintDebugMessage("Unknown state requested", FColor::Red);
		break;
	}
}

// timer handle for frame rate dependent - fires on tick
void UComboSystem::TimerHandle_frameRateDependent() //todo rename
{
	FrameCountdown = FrameCountdown - 1;

	if (FrameCountdown < 2)
	{
		switch (CurrentState)
		{
		case(EComboState::ROOT):
			//shouldn't really happen
			FrameCountdown = 1;
			PrintDebugMessage("ROOT to ROOT", FColor::Red);
			break;
		case(EComboState::STARTUP):
			//state to input
			FrameCountdown = CurrentComboNode->InputFrames;
			GetWorld()->GetTimerManager().SetTimer(FrameRateDependentModeTimer, this, &UComboSystem::TimerHandle_frameRateDependent, GetWorld()->GetDeltaSeconds(), false);
			CurrentState = EComboState::INPUT;
			PrintDebugMessage("STARTUP to INPUT", FColor::Yellow);
			break;
		case(EComboState::INPUT):
			//state to cooldown
			FrameCountdown = CurrentComboNode->CooldownFrames;
			GetWorld()->GetTimerManager().SetTimer(FrameRateDependentModeTimer, this, &UComboSystem::TimerHandle_frameRateDependent, GetWorld()->GetDeltaSeconds(), false);
			CurrentState = EComboState::COOLDOWN;
			PrintDebugMessage("INPUT to COOLDOWN", FColor::Yellow);
			break;
		case(EComboState::COOLDOWN):
			//state and node to root
			CurrentComboNode = RootNode;
			CurrentState = EComboState::ROOT;
			FrameCountdown = 1;
			PrintDebugMessage("COOLDOWN to ROOT", FColor::Yellow);
			break;
		}
	}
	else if (FrameCountdown > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(FrameRateDependentModeTimer, this, &UComboSystem::TimerHandle_frameRateDependent, GetWorld()->GetDeltaSeconds(), false);
	}
	else
	{
		PrintDebugMessage("Frame Rate Timer  was reset", FColor::Cyan);
		GetWorld()->GetTimerManager().ClearTimer(FrameRateDependentModeTimer);
	}
}

void UComboSystem::InitComboSystem(void)
{
	if (!ComboTree)
	{
		PrintDebugMessage("ComboSystem started without a ComboTree", FColor::Red);
		return;
	}
	RootNode = ComboTree->RootNode;
	CurrentComboNode = RootNode;

	// only tick if we are counting each frame, do not tick in frame-rate-independent mode 
	PrimaryComponentTick.bCanEverTick = !(ComboTree->IsFrameIndependent);
}

void UComboSystem::PrintDebugMessage(FString MessageToPrint, FColor ColorToPrint)
{
#if WITH_EDITOR
	if (IsDebugMode)
	{
		MessageToPrint = ComboSystemUniqueName.ToString() + ": " + MessageToPrint;
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, ColorToPrint, MessageToPrint);
		UE_LOG(LogComboSystem, Warning, TEXT("%s"), *MessageToPrint);
	}
#endif
}
