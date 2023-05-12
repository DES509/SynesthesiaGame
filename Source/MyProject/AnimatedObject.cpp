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
	UpdateCooldown(DeltaTime);
	CheckOnCooldown();
}

void AAnimatedObject::UpdateCooldown(float DeltaTime)
{
	curCooldown += DeltaTime;

	if (curCooldown >= cooldownDuration)
	{
		// reset
		curCooldown = 0;

		// decay
		if (bOnCooldown)
			bOnCooldown = false;
	}
}

void AAnimatedObject::TriggerPulse(bool isEnabled)
{
	if (bOnCooldown)
		return;

	if (isVisible == false) 
	{
		isVisible = true;
		MakeVisible_Event();
	}

	curTriggers++;
}

void AAnimatedObject::CheckOnCooldown()
{
	if (curTriggers >= triggersBeforeCooldown)
	{
		curTriggers = 0;
		MakeInvisible_Event();
		curCooldown = 0;
		bOnCooldown = true;
		isVisible = false;
	}
}

