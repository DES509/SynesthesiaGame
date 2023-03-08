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

UCLASS()
class METASOUNDTOOL_API AMTReceiver: public AActor
{
    GENERATED_BODY()

public:
    AMTReceiver();
    void GetCool();
    int cool = 10;

protected:
	virtual void BeginPlay();
};

class Passer
{
public:
    Passer();
    Passer(int NumVar);
    ~Passer();
    static int Num;
};