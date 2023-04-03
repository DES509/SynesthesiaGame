// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_Component.h"

// Sets default values for this component's properties
UPickUp_Component::UPickUp_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}


// Called when the game starts
void UPickUp_Component::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPickUp_Component::OnSphereBeginOverlap);
	
}


void UPickUp_Component::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AMyProjectCharacter* Character = Cast<AMyProjectCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
