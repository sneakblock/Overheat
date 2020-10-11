// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDoor.h"
#include "Curves/CurveFloat.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SceneComponent.h"
#include "DoubleDoors.generated.h"

/**
 * 
 */
UCLASS()
class OVERHEAT_API ADoubleDoors : public ABaseDoor
{
	GENERATED_BODY()


public:
	//constructor
	ADoubleDoors();

	UPROPERTY(EditAnywhere)
		USceneComponent* rootComp;

	UPROPERTY(EditAnywhere)
		USceneComponent* childSceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* openCurve;

	UPROPERTY()
	float openRotation;

	UPROPERTY()
	float closedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite = "Rotation")
		float amountToRotate;

	UTimelineComponent* timeline;
	float timelineValue;
	float openCurveValue;
	bool open;

	
	UFUNCTION()
	virtual void ControlDoor();

	virtual void ToggleDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
