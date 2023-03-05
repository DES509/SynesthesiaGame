#include "Core.h"
#include "GameFramework/Actor.h"
#include "MetaSoundVariableReceiver.generated.h"

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