#include "MetasoundExecutableOperator.h"     // TExecutableOperator class
#include "MetasoundPrimitives.h"             // ReadRef and WriteRef descriptions for bool, int32, float, and string
#include "MetasoundNodeRegistrationMacro.h"  // METASOUND_LOCTEXT and METASOUND_REGISTER_NODE macros
#include "MetasoundStandardNodesNames.h"     // StandardNodes namespace
#include "MetasoundFacade.h"				 // FNodeFacade class, eliminates the need for a fair amount of boilerplate code
#include "MetasoundParamHelper.h"            // METASOUND_PARAM and METASOUND_GET_PARAM family of macros
#include "MetasoundWave.h"
#include "UObject/UObjectBase.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "MetaSoundToolBPFunctionLibrary.h"
#include "MetasoundAssetBase.h"
#include "Kismet/GameplayStatics.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "Subsystems/EngineSubsystem.h"
#include "Templates/Function.h"
#include "MSReceiver.h"
#include "Sound/SoundWave.h"
#include "MetasoundUObjectRegistry.h"

// Required for ensuring the node is supported by all languages in engine. Must be unique per MetaSound.
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_MetaSoundSend2DLocToReceiverNode"

namespace Metasound
{
	enum class EInstrumentType
	{
		Lyre, Flute, Drum, Trumpet 
	};

	DECLARE_METASOUND_ENUM(EInstrumentType, EInstrumentType::Lyre, METASOUNDTOOL_API,
		FEnumInstrumentType, FEnumInstrumentTypeInfo, FEnumInstrumentTypeReadRef, FEnumInstrumentTypeWriteRef);

	DEFINE_METASOUND_ENUM_BEGIN(EInstrumentType, FEnumInstrumentType, "InstrumentType")
		DEFINE_METASOUND_ENUM_ENTRY(EInstrumentType::Lyre, "LyreDescription", "Lyre", "Lyre", "Lyre as the target type"),
		DEFINE_METASOUND_ENUM_ENTRY(EInstrumentType::Flute, "FluteDescription", "Flute", "Flute", "Flute as the target type"),
		DEFINE_METASOUND_ENUM_ENTRY(EInstrumentType::Drum, "DrumDescription", "Drum", "Drum", "Drum as the target type"),
		DEFINE_METASOUND_ENUM_ENTRY(EInstrumentType::Trumpet, "TrumpetDescription", "Trumpet", "Trumpet", "Trumpet as the target type"),
	DEFINE_METASOUND_ENUM_END()
	// Vertex Names - define your node's inputs and outputs here
	namespace Send2DLocToReceiverNames
	{
		METASOUND_PARAM(InputTrigger, "Input Trigger", "Input Trigger.");
		METASOUND_PARAM(InputReset, "Reset", "Reset.");
		METASOUND_PARAM(InputXValue, "Loc X", "Loc X Variable.");
        METASOUND_PARAM(InputYValue, "Loc Y", "Loc Y Variable.");
		METASOUND_PARAM(InputInstrumentTypeValue, "Ins Type", "Instrument Type Variable.");
		METASOUND_PARAM(OutputNumName, "Null", "Null.");
	}


	// Operator Class - defines the way your node is described, created and executed
	class FSend2DLocToReceiverOperator : public TExecutableOperator<FSend2DLocToReceiverOperator>
	{
		public:
			// Constructor
			FSend2DLocToReceiverOperator(
				const FTriggerReadRef& InAValue,
				const FTriggerReadRef& InBValue,
				const FFloatReadRef& InCValue,
                const FFloatReadRef& InDValue,
				const FEnumInstrumentTypeReadRef& InInsValue)
				: InputTrigger(InAValue), InputReset(InBValue), InputXValue(InCValue), InputYValue(InDValue), InputInstrumentTypeValue(InInsValue)
				, Send2DLocToReceiverOutput(FFloatWriteRef::CreateNew())
			{
			}

			// Helper function for constructing vertex interface
			static const FVertexInterface& DeclareVertexInterface()
			{
				using namespace Send2DLocToReceiverNames;

				static const FVertexInterface Interface(
					FInputVertexInterface(
						TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_TT(InputTrigger)),
						TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_TT(InputReset)),
						TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputXValue)),
                        TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputYValue)),
						TInputDataVertex<FEnumInstrumentType>(METASOUND_GET_PARAM_NAME_AND_TT(InputInstrumentTypeValue))
					),
					FOutputVertexInterface(
						TOutputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_TT(OutputNumName))
					)
				);

				return Interface;
			}

			// Retrieves necessary metadata about your node
			static const FNodeClassMetadata& GetNodeInfo()
			{
				auto CreateNodeClassMetadata = []() -> FNodeClassMetadata
				{
					FVertexInterface NodeInterface = DeclareVertexInterface();

					FNodeClassMetadata Metadata
					{
						FNodeClassName { "UE", "Send Vector2D To Receiver Node", "Audio" }, 
						1, // Major Version
						0, // Minor Version
						METASOUND_LOCTEXT("Send2DLocToReceiverDisplayName", "Send Vector2D To Receiver Node"),
						METASOUND_LOCTEXT("Send2DLocToReceiverDesc", "A simple node to Send Vector2D To Receiver"),
						PluginAuthor,
						PluginNodeMissingPrompt,
						NodeInterface,
						{ }, // Category Hierarchy 
						{ }, // Keywords for searching
						FNodeDisplayStyle{}
					};

					return Metadata;
				};

				static const FNodeClassMetadata Metadata = CreateNodeClassMetadata();
				return Metadata;
			}

			// Allows MetaSound graph to interact with your node's inputs
			virtual FDataReferenceCollection GetInputs() const override
			{
				using namespace Send2DLocToReceiverNames;

				FDataReferenceCollection InputDataReferences;

				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputTrigger), InputTrigger);
				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputReset), InputReset);
				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputXValue), InputXValue);
                InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputYValue), InputYValue);
				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputInstrumentTypeValue), InputInstrumentTypeValue);

				return InputDataReferences;
			}

			// Allows MetaSound graph to interact with your node's outputs
			virtual FDataReferenceCollection GetOutputs() const override
			{
				using namespace Send2DLocToReceiverNames;

				FDataReferenceCollection OutputDataReferences;

				OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputNumName), Send2DLocToReceiverOutput);

				return OutputDataReferences;
			}

			// Used to instantiate a new runtime instance of your node
			static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
			{
				using namespace Send2DLocToReceiverNames;

				const Metasound::FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
				const Metasound::FInputVertexInterface& InputInterface = DeclareVertexInterface().GetInputInterface();

				TDataReadReference<FTrigger> InputTrigger = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FTrigger>(InputInterface, METASOUND_GET_PARAM_NAME(InputTrigger), InParams.OperatorSettings);
				TDataReadReference<FTrigger> InputReset = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FTrigger>(InputInterface, METASOUND_GET_PARAM_NAME(InputReset), InParams.OperatorSettings);
				TDataReadReference<float> InputXValue = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputXValue), InParams.OperatorSettings);
                TDataReadReference<float> InputYValue = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputYValue), InParams.OperatorSettings);
				TDataReadReference<FEnumInstrumentType> InputInstrumentTypeValue = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FEnumInstrumentType>(InputInterface, METASOUND_GET_PARAM_NAME(InputInstrumentTypeValue), InParams.OperatorSettings);

				return MakeUnique<FSend2DLocToReceiverOperator>(InputTrigger, InputReset, InputXValue, InputYValue, InputInstrumentTypeValue);
			}

			// Primary node functionality
			void Execute()
			{
				*Send2DLocToReceiverOutput = 1;
				
				InputTrigger->ExecuteBlock(
					[](int32, int32) 
					{
					},
					[this](int32 StartFrame, int32 EndFrame)
					{
						if (bIsGateOpen)
						{
							bIsGateOpen = false;
							if(UMetaSoundToolBPFunctionLibrary::Receiver != nullptr)
							{
								switch(*InputInstrumentTypeValue)
								{
									case EInstrumentType::Lyre:
										UMetaSoundToolBPFunctionLibrary::Receiver->SendLoc.Broadcast(*InputXValue, *InputYValue, 0);
										break;
									case EInstrumentType::Flute:
										UMetaSoundToolBPFunctionLibrary::Receiver->SendLoc.Broadcast(*InputXValue, *InputYValue, 1);
										break;	
									case EInstrumentType::Drum:
										UMetaSoundToolBPFunctionLibrary::Receiver->SendLoc.Broadcast(*InputXValue, *InputYValue, 2);
										break;	
									case EInstrumentType::Trumpet:
										UMetaSoundToolBPFunctionLibrary::Receiver->SendLoc.Broadcast(*InputXValue, *InputYValue, 3);
										break;	
									default:
										break;
								}

							}
						}
					}
				);	

				InputReset->ExecuteBlock(
					[](int32, int32) 
					{
					},
					[this](int32 StartFrame, int32 EndFrame)
					{
						bIsGateOpen = true;
					}
				);	
			}

	private:

		// Inputs
		FTriggerReadRef InputTrigger;
		FTriggerReadRef InputReset;
		FFloatReadRef InputXValue;
        FFloatReadRef InputYValue;
		FEnumInstrumentTypeReadRef InputInstrumentTypeValue;
		EInstrumentType InsEnum;

		// Outputs
		FFloatWriteRef Send2DLocToReceiverOutput;

		bool bIsGateOpen = true;
	};

	// Node Class - Inheriting from FNodeFacade is recommended for nodes that have a static FVertexInterface
	class FSend2DLocToReceiver : public FNodeFacade
	{
		public:
			FSend2DLocToReceiver(const FNodeInitData& InitData)
				: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FSend2DLocToReceiverOperator>())
			{
			}
	};

	// Register node
	METASOUND_REGISTER_NODE(FSend2DLocToReceiver);
}

#undef LOCTEXT_NAMESPACE