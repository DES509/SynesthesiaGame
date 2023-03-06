#include "MetaSoundVariableReceiver.h"
#include "Kismet/GameplayStatics.h"

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