#include "MSReceiver.h"
#include "Kismet/GameplayStatics.h"
#include "MetaSoundToolBPFunctionLibrary.h"

// custom log category
//DEFINE_LOG_CATEGORY(Sandbox);

AMSReceiver* UMetaSoundToolBPFunctionLibrary::Receiver;

AMSReceiver::AMSReceiver()
{
    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
    UMetaSoundToolBPFunctionLibrary::Receiver = this;
    SetSpeed(30);
}

float AMSReceiver::GetSpeed()
{
    return speed;
}

void AMSReceiver::SetSpeed(float Value)
{
    speed = Value;
}

void AMSReceiver::BeginPlay()
{
    Super::BeginPlay();    

    Initialise();
}

void AMSReceiver::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMSReceiver::Initialise()
{

}