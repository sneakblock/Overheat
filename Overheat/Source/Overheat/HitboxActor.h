// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxActor.generated.h"

UENUM(BlueprintType)
enum class EHitboxEnum : uint8 {
	HB_PROXIMITY UMETA(DisplayName = "Proximity"),
	HB_STRIKE    UMETA(DisplayName = "Strike"),
	HB_HURTBOX   UMETA(DisplayName = "Hurtbox")
};

UCLASS()
class OVERHEAT_API AHitboxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitboxActor();

	//Notify the HitboxActorBP class instance that the hitbox is ready to be drawn.
	UFUNCTION(BlueprintCallable)
		void TriggerVisualizeHitbox();

	//Draw the hitbox to the screen to visualize it.
	//UFUNCTION(BlueprintCallable)
		//void VisualizeHitbox();

	//The hitbox enum instance.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitboxEnum hitboxType;

	//The locationto spawn the hitbox.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		FVector hitboxLocation;
	
	//The damage a hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitboxDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
