// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedObject.h"

// Sets default values
AAnimatedObject::AAnimatedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	//RootComponent = meshComponent;
}

// Called when the game starts or when spawned
void AAnimatedObject::BeginPlay()
{
	Super::BeginPlay();

	//meshComponent->SetMaterial(0, materialComponent);
}

// Called every frame
void AAnimatedObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateDecayRate(DeltaTime);
}

void AAnimatedObject::UpdateDecayRate(float DeltaTime) 
{
	curDecayDuration += DeltaTime;

	if (curDecayDuration >= maxDecayDuration) 
	{
		// reset
		curDecayDuration = 0;

		// decay
		if (curTriggers > 0)
			curTriggers--;
	}
}

void AAnimatedObject::TriggerPulse(bool isEnabled, float minDuration, float maxDuration)
{
	curTriggers++;

	if (curTriggers > maxTriggers) 
	{
		curTriggers = maxTriggers;
		return;
	}

	MakeVisible_Event();
}

bool AAnimatedObject::IsOnCooldown() 
{
	if (curTriggers >= maxTriggers) 
	{
		curTriggers = maxTriggers;
		MakeInvisible_Event();
		return true;
	}

	return false;
}

