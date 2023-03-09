#include "MetaSoundVariableReceiver.h"
#include "Kismet/GameplayStatics.h"

// custom log category
DEFINE_LOG_CATEGORY(Sandbox);

AMTReceiver* FMetaSoundVarPasser::TestObj;

AMTReceiver::AMTReceiver()
{
    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

void AMTReceiver::GetCool()
{
    UE_LOG(LogTemp, Warning, TEXT("Cool"));
}

void AMTReceiver::BeginPlay()
{
    Super::BeginPlay();    

    Initialise();
    cool = 0;
}

void AMTReceiver::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMTReceiver::Initialise()
{
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay!"));
    TSharedRef<FMetaSoundVarPasser> InitialPasser = MakeShared<FMetaSoundVarPasser>();
    InitialPasser->TestObj = this;
}

FMetaSoundVarPasser::FMetaSoundVarPasser()
{
    UE_LOG(LogTemp, Warning, TEXT("Initialise"));
}

FMetaSoundVarPasser::~FMetaSoundVarPasser()
{
    LOG_WARNING("Destroy");
}
