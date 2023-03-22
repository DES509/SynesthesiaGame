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
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_MetaSoundNoteCalculatorNode"

namespace Metasound
{
	// Vertex Names - define your node's inputs and outputs here
	namespace NoteCalculatorNodeNames
	{
		METASOUND_PARAM(NoteName, "Target Note Name", "Target Note Name.");
		METASOUND_PARAM(NoteDegreeName, "Target Note Degree", "Target Note Degree.");

		METASOUND_PARAM(NoteNumberName, "Note Number", "Note Number.");
	}


	// Operator Class - defines the way your node is described, created and executed
	class FNoteCalculatorOperator : public TExecutableOperator<FNoteCalculatorOperator>
	{
		public:
			// Constructor
			FNoteCalculatorOperator(
				const FFloatReadRef& InAValue,
				const FStringReadRef& InBValue)
				: InputA(InAValue)
				, InputB(InBValue)
				, NoteCalculatorNodeOutput(FFloatWriteRef::CreateNew())
			{
			}

			// Helper function for constructing vertex interface
			static const FVertexInterface& DeclareVertexInterface()
			{
				using namespace NoteCalculatorNodeNames;

				static const FVertexInterface Interface(
					FInputVertexInterface(
						TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(NoteDegreeName)),
						TInputDataVertexModel<FString>(METASOUND_GET_PARAM_NAME_AND_TT(NoteName))
					),
					FOutputVertexInterface(
						TOutputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(NoteNumberName))
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
						FNodeClassName { StandardNodes::Namespace, "Note Calculator Node", StandardNodes::AudioVariant }, 
						1, // Major Version
						0, // Minor Version
						METASOUND_LOCTEXT("NoteCalculatorNodeDisplayName", "Note Calculator Node"),
						METASOUND_LOCTEXT("NoteCalculatorNodeDesc", "A simple node to demonstrate how to create new MetaSound nodes in C++. Adds two floats together"),
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
				using namespace NoteCalculatorNodeNames;

				FDataReferenceCollection InputDataReferences;

				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(NoteDegreeName), InputA);
				InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(NoteName), InputB);

				return InputDataReferences;
			}

			// Allows MetaSound graph to interact with your node's outputs
			virtual FDataReferenceCollection GetOutputs() const override
			{
				using namespace NoteCalculatorNodeNames;

				FDataReferenceCollection OutputDataReferences;

				OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(NoteNumberName), NoteCalculatorNodeOutput);

				return OutputDataReferences;
			}

			// Used to instantiate a new runtime instance of your node
			static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
			{
				using namespace NoteCalculatorNodeNames;

				const Metasound::FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
				const Metasound::FInputVertexInterface& InputInterface = DeclareVertexInterface().GetInputInterface();

				TDataReadReference<float> InputA = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(NoteDegreeName), InParams.OperatorSettings);
				TDataReadReference<FString> InputB = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FString>(InputInterface, METASOUND_GET_PARAM_NAME(NoteName), InParams.OperatorSettings);

				return MakeUnique<FNoteCalculatorOperator>(InputA, InputB);
			}

			// Primary node functionality
			void Execute()
			{
				*NoteCalculatorNodeOutput = (*InputA+2)*12 + AlphabetToNum(*InputB);
			}

	private:

		// Inputs
		FFloatReadRef InputA;
		FStringReadRef InputB;

		// Outputs
		FFloatWriteRef NoteCalculatorNodeOutput;

        float AlphabetToNum(FString note)
		{
			const TCHAR* chars = *note;
			switch (chars[0]) 
			{
				case 'C':
					return 0.0f;
					break;
				case 'D':
					return 2.0f;
					break;
				case 'E':
					return 4.0f;
					break;
				case 'F':
					return 5.0f;
					break;
				case 'G':
					return 7.0f;
					break;
				case 'A':
					return 9.0f;
					break;
				case 'B':
					return 11.0f;
					break;
				case 'c':
					return 0.0f;
					break;
				case 'd':
					return 2.0f;
					break;
				case 'e':
					return 4.0f;
					break;
				case 'f':
					return 5.0f;
					break;
				case 'g':
					return 7.0f;
					break;
				case 'a':
					return 9.0f;
					break;
				case 'b':
					return 11.0f;
					break;
				default: break;
			}
			return 0;
		}
	};

	// Node Class - Inheriting from FNodeFacade is recommended for nodes that have a static FVertexInterface
	class FNoteCalculatorNode : public FNodeFacade
	{
		public:
			FNoteCalculatorNode(const FNodeInitData& InitData)
				: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FNoteCalculatorOperator>())
			{
			}
	};

	// Register node
	METASOUND_REGISTER_NODE(FNoteCalculatorNode);
}

#undef LOCTEXT_NAMESPACE