#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetaSoundVariableReceiver.generated.h"

UCLASS()
class METASOUNDTOOL_API AMTReceicer: public AActor
{
    GENERATED_BODY()

    public:
    AMTReceicer();
    void GetVariable();

    protected:
	virtual void BeginPlay();
};