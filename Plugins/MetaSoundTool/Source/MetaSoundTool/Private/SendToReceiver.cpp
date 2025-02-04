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
#include "Subsystems/EngineSubsystem.h"
#include "Templates/Function.h"
#include "MSReceiver.h"
#include "Sound/SoundWave.h"
#include "MetasoundUObjectRegistry.h"

// Required for ensuring the node is supported by all languages in engine. Must be unique per MetaSound.
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_MetaSoundSendToReceiverNode"

namespace Metasound
{
	// Vertex Names - define your node's inputs and outputs here
	namespace SendToReceiverNames
	{
		METASOUND_PARAM(InputTrigger, "Input Trigger", "Input Trigger.");
		METASOUND_PARAM(InputReset, "Reset", "Reset.");
		METASOUND_PARAM(InputValue, "Float", "Float Variable.");
		METASOUND_PARAM(OutputNumName, "Null", "Null.");
	}


	// Operator Class - defines the way your node is described, created and executed
	class FSendToReceiverOperator : public TExecutableOperator<FSendToReceiverOperator>
	{
		public:
			// Constructor
			FSendToReceiverOperator(
				const FTriggerReadRef& InAValue,
				const FTriggerReadRef& InBValue,
				const FFloatReadRef& InCValue)
				: InputTrigger(InAValue), InputReset(InBValue), InputValue(InCValue)
				, SendToReceiverOutput(FFloatWriteRef::CreateNew())
			{
			}

			// Helper function for constructing vertex interface
			static const FVertexInterface& DeclareVertexInterface()
			{
				using namespace SendToReceiverNames;

				static const FVertexInterface Interface(
					FInputVertexInterface(
						TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_TT(InputTrigger)),
						TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_TT(InputReset)),
						TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputValue))
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
						FNodeClassName { "UE", "Send Single Float To Receiver Node", "Audio" }, 
						1, // Major Version
						0, // Minor Version
						METASOUND_LOCTEXT("SendToReceiverDisplayName", "Send Single Float To Receiver Node"),
						METASOUND_LOCTEXT("SendToReceiverDesc", "A simple node to demonstrate how to create new MetaSound nodes in C++. Adds two floats together"),
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
				using namespace SendToReceiverNames;

				FDataReferenceCollection InputDataReferences;

				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputTrigger), InputTrigger);
				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputReset), InputReset);
				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputValue), InputValue);

				return InputDataReferences;
			}

			// Allows MetaSound graph to interact with your node's outputs
			virtual FDataReferenceCollection GetOutputs() const override
			{
				using namespace SendToReceiverNames;

				FDataReferenceCollection OutputDataReferences;

				OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputNumName), SendToReceiverOutput);

				return OutputDataReferences;
			}

			// Used to instantiate a new runtime instance of your node
			static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
			{
				using namespace SendToReceiverNames;

				const Metasound::FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
				const Metasound::FInputVertexInterface& InputInterface = DeclareVertexInterface().GetInputInterface();

				TDataReadReference<FTrigger> InputTrigger = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FTrigger>(InputInterface, METASOUND_GET_PARAM_NAME(InputTrigger), InParams.OperatorSettings);
				TDataReadReference<FTrigger> InputReset = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FTrigger>(InputInterface, METASOUND_GET_PARAM_NAME(InputReset), InParams.OperatorSettings);
				TDataReadReference<float> InputValue = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputValue), InParams.OperatorSettings);

				return MakeUnique<FSendToReceiverOperator>(InputTrigger, InputReset, InputValue);
			}

			// Primary node functionality
			void Execute()
			{
				*SendToReceiverOutput = 1;
				
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
								UMetaSoundToolBPFunctionLibrary::Receiver->SendFloat.Broadcast(*InputValue);
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
		FFloatReadRef InputValue;

		// Outputs
		FFloatWriteRef SendToReceiverOutput;

		bool bIsGateOpen = true;
	};

	// Node Class - Inheriting from FNodeFacade is recommended for nodes that have a static FVertexInterface
	class FSendToReceiver : public FNodeFacade
	{
		public:
			FSendToReceiver(const FNodeInitData& InitData)
				: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FSendToReceiverOperator>())
			{
			}
	};

	// Register node
	METASOUND_REGISTER_NODE(FSendToReceiver);
}

#undef LOCTEXT_NAMESPACE