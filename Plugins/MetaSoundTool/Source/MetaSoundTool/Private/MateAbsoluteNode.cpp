#include "MetasoundExecutableOperator.h"     // TExecutableOperator class
#include "MetasoundPrimitives.h"             // ReadRef and WriteRef descriptions for bool, int32, float, and string
#include "MetasoundNodeRegistrationMacro.h"  // METASOUND_LOCTEXT and METASOUND_REGISTER_NODE macros
#include "MetasoundStandardNodesNames.h"     // StandardNodes namespace
#include "MetasoundFacade.h"				         // FNodeFacade class, eliminates the need for a fair amount of boilerplate code
#include "MetasoundParamHelper.h"            // METASOUND_PARAM and METASOUND_GET_PARAM family of macros
#include "MetasoundWave.h"
#include "UObject/UObjectBase.h"
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "MetasoundAssetBase.h"
#include "Subsystems/EngineSubsystem.h"
#include "Templates/Function.h"
#include "Sound/SoundWave.h"
#include "MetasoundUObjectRegistry.h"

// Required for ensuring the node is supported by all languages in engine. Must be unique per MetaSound.
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_MetaSoundMathAbs"

namespace Metasound
{
	// Vertex Names - define your node's inputs and outputs here
	namespace MathAbsNames
	{
		METASOUND_PARAM(InputNumName, "Input Number", "Input Number.");
		METASOUND_PARAM(OutputNumName, "Abs Number", "Abs Num.");
	}


	// Operator Class - defines the way your node is described, created and executed
	class FMathAbsOperator : public TExecutableOperator<FMathAbsOperator>
	{
		public:
			// Constructor
			FMathAbsOperator(
				const FFloatReadRef& InAValue)
				: InputA(InAValue)
				, MathAbsOutput(FFloatWriteRef::CreateNew())
			{
			}

			// Helper function for constructing vertex interface
			static const FVertexInterface& DeclareVertexInterface()
			{
				using namespace MathAbsNames;

				static const FVertexInterface Interface(
					FInputVertexInterface(
						TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputNumName))
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
						FNodeClassName { "UE", "Math Abs Node", "Audio" }, 
						1, // Major Version
						0, // Minor Version
						METASOUND_LOCTEXT("MathAbsDisplayName", "Math Abs Node"),
						METASOUND_LOCTEXT("MathAbsDesc", "A simple node to demonstrate how to create new MetaSound nodes in C++. Adds two floats together"),
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
				using namespace MathAbsNames;

				FDataReferenceCollection InputDataReferences;

				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputNumName), InputA);

				return InputDataReferences;
			}

			// Allows MetaSound graph to interact with your node's outputs
			virtual FDataReferenceCollection GetOutputs() const override
			{
				using namespace MathAbsNames;

				FDataReferenceCollection OutputDataReferences;

				OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputNumName), MathAbsOutput);

				return OutputDataReferences;
			}

			// Used to instantiate a new runtime instance of your node
			static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
			{
				using namespace MathAbsNames;

				const Metasound::FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
				const Metasound::FInputVertexInterface& InputInterface = DeclareVertexInterface().GetInputInterface();

				TDataReadReference<float> InputA = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputNumName), InParams.OperatorSettings);
				return MakeUnique<FMathAbsOperator>(InputA);
			}

			// Primary node functionality
			void Execute()
			{
				*MathAbsOutput = *InputA < 0? *InputA * -1: *InputA;
			}

	private:

		// Inputs
		FFloatReadRef InputA;

		// Outputs
		FFloatWriteRef MathAbsOutput;
	};

	// Node Class - Inheriting from FNodeFacade is recommended for nodes that have a static FVertexInterface
	class FMathAbs : public FNodeFacade
	{
		public:
			FMathAbs(const FNodeInitData& InitData)
				: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FMathAbsOperator>())
			{
			}
	};

	// Register node
	METASOUND_REGISTER_NODE(FMathAbs);
}

#undef LOCTEXT_NAMESPACE