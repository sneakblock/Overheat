/*
* Copyright (c) 2018 Jonathan Friesen <jonathan.t.friesen@gmail.com>
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboTree.h"
#include "Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "MovementModeEnum.h"
#include "ComboNode.h"
#include <map>
#include "ComboSystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogComboSystem, Log, All);

UENUM(Category = "ComboSystem")
enum class EComboState : uint8 {
	ROOT,
	STARTUP,
	INPUT,
	COOLDOWN
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMBOSYSTEM_API UComboSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComboSystem();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ComboSystem")
		UComboTree *ComboTree;

	//TODO: UFUNCTION swap to tree

	// If combosystem is currently in 'input frames', advance along graph according to EnumValue
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		bool AdvanceToNextCombo(uint8 EnumValue);

	// Return true if the combo system is in 'input frames'
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		bool IsNextComboAvailable();

	// Get the name associated with the current combo
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		FString GetCurrentComboName();

	// Get the launch vector associated with the current combo
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		FVector GetCurrentComboLaunch();

	// Get the enemy launch vector associated with the current combo
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		FVector GetCurrentComboEnemyLaunch();

	// Get the multiplier associated with the current combo
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		float GetCurrentComboMultiplier();

	// Set the movement mode of the combo system
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		bool SetMovementMode(uint8 EnumValue);

	// Move the combosystem back to root. Return the name of the interrupted combo.
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		FString Reset();

	// Set ineternal HasHitLanded to True
	UFUNCTION(BlueprintCallable, Category = "ComboSystem")
		void LandHit();

	UPROPERTY(BlueprintReadOnly, Category = "ComboSystem")
		UComboNode * CurrentComboNode;

	UPROPERTY(BlueprintReadOnly, Category = "ComboSystem")
		UComboNode * RootNode;

	UPROPERTY(BlueprintReadOnly, Category = "ComboSystem")
		EComboState CurrentState;

	UPROPERTY(BlueprintReadOnly, Category = "ComboSystem")
		ECombo_MovementMode CurrentMode;

	UPROPERTY(BlueprintReadOnly, Category = "ComboSystem")
		bool HasHitLanded = false;

	//TODO: get frame countdown - featurecreep, not necessary

	// This activates or deactivates debug messages from the ComboSystem. ComboSystem debug messages are only printed if this is true and if the project is built WITH_EDITOR defined, so messages cannot accidentally appear in release builds.
	UPROPERTY(BlueprintReadWrite, Category = "ComboSystem")
		bool IsDebugMode = false;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FCriticalSection ComboMutex;

	int FrameCountdown;

	// timer used to decrement frames
	FTimerHandle FrameRateDependentModeTimer;

	// timer used to go between states
	FTimerHandle FrameRateIndependentModeTimer;

	void SetNextCombo_frameRateDependent(UComboNode *NextComboNode);

	void SetNextCombo_frameRateIndependent(UComboNode *NextComboNode);

	void TimerHandle_frameRateIndependent();

	void TimerHandle_frameRateDependent();

	void InitComboSystem();

	void PrintDebugMessage(FString MessageToPrint, FColor ColorToPrint);

	std::map<EComboState, FString> ComboStateNames = { 
		{EComboState::ROOT, FString("ROOT")},
		{EComboState::STARTUP, FString("STARTUP")},
		{EComboState::INPUT, FString("INPUT")}, 
		{EComboState::COOLDOWN, FString("COOLDOWN")}};

	FName ComboSystemUniqueName;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
