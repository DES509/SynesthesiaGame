#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "MetasoundEnum.h"
#include "MSReceiver.generated.h"


// custom log category
DECLARE_LOG_CATEGORY_EXTERN(Sandbox, Log, All);

// custom log macro
#define LOG(x, ...) UE_LOG(Sandbox, Log, TEXT(x), __VA_ARGS__)
#define LOG_WARNING(x, ...) UE_LOG(Sandbox, Warning, TEXT(x), __VA_ARGS__)
#define LOG_ERROR(x, ...) UE_LOG(Sandbox, Error, TEXT(x), __VA_ARGS__)

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIntDelegate, float, ValueA);

//Lyre = 0, Flute = 1, Drum = 2, Trumpet = 3
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FVector2DDelegate, float, ValueX, float, ValueY, int, TypeOfInstrument);

UCLASS()
class METASOUNDTOOL_API AMSReceiver: public AActor
{
    GENERATED_BODY()

public:
    AMSReceiver();

    UPROPERTY(BlueprintAssignable)
    FIntDelegate SendFloat;
    UPROPERTY(BlueprintAssignable)
    FVector2DDelegate SendLoc;
    float GetSpeed();
    void SetSpeed(float Value);

private:
    float speed;


protected:
	virtual void BeginPlay();
    virtual void Tick(float DeltaTime);

private:
    void Initialise();

private:
    //TSharedPtr<FMetaSoundVarPasser> Test = new FMetaSoundVarPasser();
};