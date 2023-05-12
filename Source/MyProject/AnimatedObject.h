// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "AnimatedObject.generated.h"


	// enum to track type of animated object
UENUM(BlueprintType) enum class EType : uint8 { ROCK, WATER, TREE, INSTRUMENT };


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

	// is on cooldown check
	void CheckOnCooldown();

	bool isVisible = false;

	// current trigger count tracking
		int curTriggers = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// current decay time tracking
	float curCooldown = 0;

	// enum to track type of animated object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animated Material")
		TEnumAsByte<EType> objectType;

	// maximum triggers before object enters cooldown and becomes inactive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animated Material")
		int triggersBeforeCooldown = 5;


	// how long object is on cooldown for
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animated Material")
		float cooldownDuration = 2;

	// if object is on cooldown do not allow triggering another pulse
	bool bOnCooldown = false;

	// update decay rate
	void UpdateCooldown(float DeltaTime);

	// function to trigger pusle
	// isEnabled = enabled target status of object
	void TriggerPulse(bool isEnabled);


	// blueprint event to make visible
	UFUNCTION(BlueprintImplementableEvent, Category = "Animated Material")
		void MakeVisible_Event();

	// blueprint event to make invvisible
	UFUNCTION(BlueprintImplementableEvent, Category = "Animated Material")
		void MakeInvisible_Event();
};
