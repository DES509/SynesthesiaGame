#include "MetaSoundVariableReceiver.h"
#include "Kismet/GameplayStatics.h"

// custom log category
DEFINE_LOG_CATEGORY(Sandbox);

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
    AActor* Actor = UGameplayStatics::GetActorOfClass(GEngine->GetWorldContexts()[0].World(), AMTReceiver::StaticClass());
    AMTReceiver* TargetReceiver = Cast<AMTReceiver>(Actor);
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay!"));
    //Test->Num = 10;
}

void AMTReceiver::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Create a shared reference to a new node 
    //LOG_WARNING("Tick %i", Test->Num);
}

FMetaSoundVarPasser::FMetaSoundVarPasser()
{
    UE_LOG(LogTemp, Warning, TEXT("Initialise"));
}

FMetaSoundVarPasser::FMetaSoundVarPasser(int NumVar)
{
    Num = NumVar;
}

FMetaSoundVarPasser::~FMetaSoundVarPasser()
{
    LOG_WARNING("Destroy %i", Num);
}