// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDoor.h"
#include "DoubleDoors.generated.h"

/**
 * 
 */
UCLASS()
class OVERHEAT_API ADoubleDoors : public ABaseDoor
{
	GENERATED_BODY()

public:
	ADoubleDoors();

	UPROPERTY(BlueprintReadWrite, )
	UStaticMesh* LeftDoor;

	UPROPERTY()
	UStaticMesh* RightDoor;

	UFUNCTION(BlueprintCallable)
	virtual void OpenDoor() override;

	UFUNCTION(BlueprintCallable)
	virtual void CloseDoor() override;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
