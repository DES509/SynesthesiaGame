#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "MetaSoundVariableReceiver.generated.h"


// custom log category
DECLARE_LOG_CATEGORY_EXTERN(Sandbox, Log, All);

// custom log macro
#define LOG(x, ...) UE_LOG(Sandbox, Log, TEXT(x), __VA_ARGS__)
#define LOG_WARNING(x, ...) UE_LOG(Sandbox, Warning, TEXT(x), __VA_ARGS__)
#define LOG_ERROR(x, ...) UE_LOG(Sandbox, Error, TEXT(x), __VA_ARGS__)

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIntDelegate, int, ValueA);

UCLASS()
class METASOUNDTOOL_API AMTReceiver: public AActor
{
    GENERATED_BODY()

public:
    AMTReceiver();
    void GetCool();
    int cool = 10;
    FIntDelegate SendInt;


protected:
	virtual void BeginPlay();
    virtual void Tick(float DeltaTime);

private:
    void Initialise();

private:
    //TSharedPtr<FMetaSoundVarPasser> Test = new FMetaSoundVarPasser();
};

class FMetaSoundVarPasser
{
public:
    FMetaSoundVarPasser();
    ~FMetaSoundVarPasser();
    static AMTReceiver* TestObj;
};