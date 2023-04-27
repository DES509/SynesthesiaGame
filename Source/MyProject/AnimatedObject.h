// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "AnimatedObject.generated.h"


	// enum to track type of animated object
UENUM(BlueprintType) enum class EType : uint8 { ROCK, WATER, TREE };


UCLASS()
class MYPROJECT_API AAnimatedObject : public AActor
{
	GENERATED_BODY()
	


public:	
	// Sets default values for this actor's properties
	AAnimatedObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* meshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// if object is on cooldown do not allow triggering another pulse
	bool bOnCooldown = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// current decay time tracking
	float curDecayDuration = 0;

	// enum to track type of animated object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animated Material")
		TEnumAsByte<EType> objectType;

	// maximum triggers before object becomes inactive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animated Material")
		uint8 maxTriggers = 5;

	// current trigger count tracking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animated Material")
		uint8 curTriggers = 0;

	// time in seconds for 1 trigger to decay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animated Material")
		float maxDecayDuration = 2;

	// is on cooldown getter
	bool IsOnCooldown();

	// update decay rate
	void UpdateDecayRate(float DeltaTime);

	// function to trigger pusle
	// isEnabled = enabled target status of object
	// minDuration = minimum duration of pulse in seconds
	// maxDuration = maximum duration of pulse in seconds
	void TriggerPulse(bool isEnabled, float minDuration, float maxDuration);

	// blueprint event to trigger pusle
	UFUNCTION(BlueprintImplementableEvent, Category = "Animated Material")
		void TriggerPulse_Event(bool isEnabled, float minDuration, float maxDuration);
};
