// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleDoors.h"

// Sets default values
ADoubleDoors::ADoubleDoors()
{
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = rootComp;
	childSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ChildSceneComponent"));
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	RightDoor->AttachTo(rootComp);
	LeftDoor->AttachTo(childSceneComponent);
	childSceneComponent->AttachTo(rootComp);
	RightDoor->SetMobility(EComponentMobility::Movable);
	LeftDoor->SetMobility(EComponentMobility::Movable);
	//RightDoor->SetupAttachment(RootComponent);
	//LeftDoor->SetupAttachment(RootComponent);
	

	amountToRotate = 150.0f;
	open = true;
	UE_LOG(LogTemp, Warning, TEXT("So annoying"));

}

// Called when the game starts or when spawned
void ADoubleDoors::BeginPlay()
{
	Super::BeginPlay();

	if (openCurve) {

		//callback that gets run while the timeline is running
		FOnTimelineFloat TimelineCallback;
		//callback that gets run at the end of the timeline
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("ControlDoor"));
		//TimelineFinishedCallback.BindUFunction(this, FName("ToggleDoor"));

		timeline = NewObject<UTimelineComponent>(this, FName("DoorAnimation"));
		timeline->AddInterpFloat(openCurve, TimelineCallback);
		timeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
		timeline->RegisterComponent();
	}

}

// Called every frame
void ADoubleDoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (timeline != NULL) {
		timeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}

}

void ADoubleDoors::ControlDoor() {
	timelineValue = timeline->GetPlaybackPosition();
	//depending on where the timeline is, get the corresponding float value
	openCurveValue = amountToRotate * openCurve->GetFloatValue(timelineValue);
	FQuat NewRotation = FQuat(FRotator(0.0f, openCurveValue, 0.0f));
	RightDoor->SetRelativeRotation(NewRotation);
	LeftDoor->SetRelativeRotation(NewRotation.Inverse());

}

void ADoubleDoors::ToggleDoor() {
	UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
	if (open) {
		if (timeline != NULL) {
			timeline->PlayFromStart();
		}
		open = !open;
	}
	else {
		if (timeline != NULL) {
			timeline->Reverse();
		}
		open = !open;
	}
}