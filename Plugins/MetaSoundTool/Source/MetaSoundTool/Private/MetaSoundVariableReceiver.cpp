#include "MetaSoundVariableReceiver.h"
#include "Kismet/GameplayStatics.h"

// custom log category
DEFINE_LOG_CATEGORY(Sandbox);

AMTReceiver::AMTReceiver()
{

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
}

Passer::Passer()
{
    UE_LOG(LogTemp, Warning, TEXT("Initialise"));
}

int Passer::Num = 0;

Passer::Passer(int NumVar)
{
    Num = NumVar;
}

Passer::~Passer()
{
    LOG_WARNING("Destroy %i", Num);
}