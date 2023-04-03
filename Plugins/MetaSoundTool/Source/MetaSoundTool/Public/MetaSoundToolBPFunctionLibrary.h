#pragma once


#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/UMG/Public/UMG.h"
#include "MetaSoundVariableReceiver.h"
#include "MetaSoundTool.h"
#include "MetaSoundToolBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class METASOUNDTOOL_API UMetaSoundToolBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:

	static AMTReceiver* Receiver;
	
	private:
};