// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnReticle.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
//#include "Engine/Engine.h"

ULockOnReticle::ULockOnReticle()
{
	// Enable ticking
	PrimaryComponentTick.bCanEverTick = true;
	SetVisibility(false);
}

void ULockOnReticle::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.5f, FColor::Red, TEXT("Tick"));
	float CurrFracOfHealth = GetTargetFractionOfHealth();
	if (FractionOfHealth != CurrFracOfHealth)
	{
		FractionOfHealth = CurrFracOfHealth;
		UpdateHealthMIDParam();
	}
}

void ULockOnReticle::AttachToNewTarget_Implementation(class ACharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		static FName SocketName = TEXT("Target");
		static FAttachmentTransformRules AttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepRelative,
				EAttachmentRule::KeepWorld,
				true
			);

		this->AttachToComponent(TargetCharacter->GetMesh(), AttachmentTransformRules, SocketName);
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.5f, FColor::Red, TEXT("Attached to Character Mesh"));

		PrimaryComponentTick.SetTickFunctionEnable(true);
		SetVisibility(true);

		// Create Reticle_MID if it does not exist
		if (!Reticle_MID)
		{
			Reticle_MID = CreateAndSetMaterialInstanceDynamic(0);
		}
		else
		{
			// Reset fraction of health parameter
			FractionOfHealth = 1.f;
			UpdateHealthMIDParam();
		}
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.5f, FColor::Red, TEXT("Visible"));
	}
}

void ULockOnReticle::DetachFromTarget_Implementation()
{
	static FDetachmentTransformRules DetachmentTransformRules(
			EDetachmentRule::KeepRelative,
			false
		);
	this->DetachFromComponent(DetachmentTransformRules);

	PrimaryComponentTick.SetTickFunctionEnable(false);
	SetVisibility(false);
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.5f, FColor::Red, TEXT("Invisible"));
}

void ULockOnReticle::UpdateHealthMIDParam()
{
	if (Reticle_MID) {
		static FName ParamName = TEXT("FractionOfHealth");
		Reticle_MID->SetScalarParameterValue(ParamName, FractionOfHealth);
	}
}