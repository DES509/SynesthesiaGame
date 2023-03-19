// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "AnimatedObject.generated.h"

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


	UFUNCTION(BlueprintImplementableEvent, Category = "Material")
		void TriggerPulse(float minDuration, float maxDuration);
};
