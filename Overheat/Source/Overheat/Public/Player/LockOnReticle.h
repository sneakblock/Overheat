// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MaterialBillboardComponent.h"
#include "LockOnReticle.generated.h"

/**
 * Material billboard component that displays which enemy the player is locked onto and that enemy's health.
 * Derive into a blueprint, override the abstract methods, and set the UPROPERTY() variables to properly
 * instantiate the class.
 */
UCLASS(Abstract, Blueprintable)
class OVERHEAT_API ULockOnReticle : public UMaterialBillboardComponent
{
	GENERATED_BODY()

public:
	ULockOnReticle();

    /**
	 * Update the FractionOfHealth parameter of the reticle if 
	 * it no longer reflects the target's health.
	 */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Attach this component to mesh of target character at socket
	 * named "Target". Turn on ticking and visibility of reticle material billboard.
	 * Create dynamic material instance if it does not exist; otherwise reset
	 * the health values.
	 * Implement in blueprint child class to set lock-on target ref.
	 * 
	 * @param Target - target character to lock on to
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character")
	void AttachToNewTarget(class ACharacter* TargetCharacter);

	virtual void AttachToNewTarget_Implementation(class ACharacter* TargetCharacter);

	/**
	 * Detach this component from the lock-on target.
	 * Turn off ticking and visibility of reticle material billboard.
	 * Implement in blueprint child class to nullify lock-on target ref.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character")
	void DetachFromTarget();

	virtual void DetachFromTarget_Implementation();

	/**
	 * Helper to construct and register the ULockOnReticle component.
	 * Attach the comopnent using AttachToNewTarget
	 * 
	 * @return A pointer to the constructed reticle component.
	 */
	//UFUNCTION(BlueprintCallable)
	//static ULockOnReticle* ConstructLockOnReticle();

protected:
	/**
	 * Helper method for retrieving the lock on target's fraction of health.
	 * Must be implemented in blueprint child class.
	 * 
	 * @return Fraction of target's health remaining if reference is valid, otherwise -1.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	float GetTargetFractionOfHealth() const;

	/**
	 * Helper method to set the value of FractionOfHealth into 
	 * the scalar parameter of the same name in Reticle_MID.
	 */
	void UpdateHealthMIDParam();

private:
	/** Pointer dynamic material instance of reticle */
	UMaterialInstanceDynamic* Reticle_MID;

	/** current value of FractionOfHealth parameter in Reticle_MID */
	float FractionOfHealth = 1.f;
};
