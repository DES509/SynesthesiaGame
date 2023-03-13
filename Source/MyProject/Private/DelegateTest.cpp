// Fill out your copyright notice in the Description page of Project Settings.

#include "DelegateTest.h"
#include "MetaSoundToolBPFunctionLibrary.h"

// Sets default values
ADelegateTest::ADelegateTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADelegateTest::BeginPlay()
{
	Super::BeginPlay();
	BindToDelegate();
}

void ADelegateTest::BindToDelegate()
{
	if(UMetaSoundToolBPFunctionLibrary::I != nullptr)
	{
		UMetaSoundToolBPFunctionLibrary::I->SendInt.AddDynamic(this, &ADelegateTest::PrintFloat);
	}
}

// Called every frame
void ADelegateTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADelegateTest::PrintFloat(float Num)
{
	UE_LOG(LogTemp, Warning, TEXT("Broadcast: %f"), Num);
}