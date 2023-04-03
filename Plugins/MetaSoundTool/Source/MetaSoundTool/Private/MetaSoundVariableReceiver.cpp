#include "MetaSoundVariableReceiver.h"
#include "Kismet/GameplayStatics.h"
#include "MetaSoundToolBPFunctionLibrary.h"

// custom log category
DEFINE_LOG_CATEGORY(Sandbox);

AMTReceiver* UMetaSoundToolBPFunctionLibrary::Receiver;

AMTReceiver::AMTReceiver()
{
    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
    UMetaSoundToolBPFunctionLibrary::Receiver = this;
    SetSpeed(30);
}

float AMTReceiver::GetSpeed()
{
    return speed;
}

void AMTReceiver::SetSpeed(float Value)
{
    speed = Value;
}

void AMTReceiver::BeginPlay()
{
    Super::BeginPlay();    

    Initialise();
}

void AMTReceiver::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMTReceiver::Initialise()
{

}