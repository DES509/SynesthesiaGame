#include "MetaSoundVariableReceiver.h"

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
}