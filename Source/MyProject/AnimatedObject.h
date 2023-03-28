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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// enum to track type of animated object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animated Material")
		TEnumAsByte<EType> objectType;
	
	// blueprint event to trigger pusle
	// isEnabled = enabled target status of object
	// minDuration = minimum duration of pulse in seconds
	// maxDuration = maximum duration of pulse in seconds
	UFUNCTION(BlueprintImplementableEvent, Category = "Animated Material")
		void TriggerPulse(bool isEnabled, float minDuration, float maxDuration);
};
