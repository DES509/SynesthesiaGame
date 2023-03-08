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
#include "MetasoundAssetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/EngineSubsystem.h"
#include "Templates/Function.h"
#include "MetaSoundVariableReceiver.h"
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
		METASOUND_PARAM(OutputNumName, "Abs Number", "Abs Num.");
	}


	// Operator Class - defines the way your node is described, created and executed
	class FSendToReceiverOperator : public TExecutableOperator<FSendToReceiverOperator>
	{
		public:
			// Constructor
			FSendToReceiverOperator(
				const FTriggerReadRef& InAValue)
				: InputA(InAValue)
				, SendToReceiverOutput(FFloatWriteRef::CreateNew())
			{
			}

			// Helper function for constructing vertex interface
			static const FVertexInterface& DeclareVertexInterface()
			{
				using namespace SendToReceiverNames;

				static const FVertexInterface Interface(
					FInputVertexInterface(
						TInputDataVertexModel<FTrigger>(METASOUND_GET_PARAM_NAME_AND_TT(InputTrigger))
					),
					FOutputVertexInterface(
						TOutputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(OutputNumName))
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
						FNodeClassName { StandardNodes::Namespace, "Send To Receiver", StandardNodes::AudioVariant }, 
						1, // Major Version
						0, // Minor Version
						METASOUND_LOCTEXT("SendToReceiverDisplayName", "Send To Receiver"),
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

				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputTrigger), InputA);

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

				TDataReadReference<FTrigger> InputA = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FTrigger>(InputInterface, METASOUND_GET_PARAM_NAME(InputTrigger), InParams.OperatorSettings);
				return MakeUnique<FSendToReceiverOperator>(InputA);
			}

			// Primary node functionality
			void Execute()
			{
				*SendToReceiverOutput = 1;
				if(flag == false)
				{
					flag = true;
					UWorld* World = GEngine->GetWorldContexts()[0].World();
					if(World!=nullptr)
					{
						Passer* A = new Passer(12);
						delete A;
						// AActor* Actor = UGameplayStatics::GetActorOfClass(World, AMTReceiver::StaticClass());
						// AMTReceiver* TargetReceiver = Cast<AMTReceiver>(Actor);
						// if(TargetReceiver != nullptr)
						// {
						// 	TargetReceiver->GetCool();
						// }
					}
				}
			}

	private:

		// Inputs
		FTriggerReadRef InputA;

		// Outputs
		FFloatWriteRef SendToReceiverOutput;

		bool flag = false;
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