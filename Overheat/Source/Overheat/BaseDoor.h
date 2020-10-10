// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDoor.generated.h"

UCLASS()
class OVERHEAT_API ABaseDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseDoor();
	UFUNCTION(BlueprintCallable)
	virtual void OpenDoor();

	UFUNCTION(BlueprintCallable)
	virtual void CloseDoor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
