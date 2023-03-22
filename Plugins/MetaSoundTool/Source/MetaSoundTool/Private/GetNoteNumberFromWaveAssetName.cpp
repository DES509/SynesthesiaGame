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
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_MetaSoundGetNoteNumFromName"

namespace Metasound
{
	// Vertex Names - define your node's inputs and outputs here
	namespace GetNoteNumFromName
	{
		// METASOUND_PARAM(InputAValue, "A", "Input value A.");
		// METASOUND_PARAM(InputBValue, "B", "Input value B.");
        METASOUND_PARAM(InputWaveAssetName, "Sample", "Sound Wave Input");
		METASOUND_PARAM(OutputNoteNumberName, "Note Number", "Note Number");
	}
    namespace StandardNodes
	{
		const FName Namespace = "UE";
		const FName AudioVariant = "Audio";
	}

	// Operator Class - defines the way your node is described, created and executed
	class FGetNoteNumFromNameOperator : public TExecutableOperator<FGetNoteNumFromNameOperator>
	{
		public:
			// Constructor
			FGetNoteNumFromNameOperator(
				// const FFloatReadRef& InAValue,
				// const FFloatReadRef& InBValue,
                const FWaveAssetReadRef& WaveAsset):
				//   InputA(InAValue)
				// , InputB(InBValue)
                WaveAsset(WaveAsset)
				, OutputNoteNumber(FFloatWriteRef::CreateNew())
			{
			}

			// Helper function for constructing vertex interface
			static const FVertexInterface& DeclareVertexInterface()
			{
				using namespace GetNoteNumFromName;

				static const FVertexInterface Interface(
					FInputVertexInterface(
						// TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputAValue)),
						// TInputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(InputBValue)),
                        TInputDataVertexModel<FWaveAsset>(METASOUND_GET_PARAM_NAME_AND_TT(InputWaveAssetName))
					),
					FOutputVertexInterface(
						TOutputDataVertexModel<float>(METASOUND_GET_PARAM_NAME_AND_TT(OutputNoteNumberName))
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
						FNodeClassName { StandardNodes::Namespace, "GetNoteNumFromName Node", StandardNodes::AudioVariant }, 
						1, // Major Version
						0, // Minor Version
						METASOUND_LOCTEXT("GetNoteNumFromNameDisplayName", "GetNoteNumFromName Node"),
						METASOUND_LOCTEXT("GetNoteNumFromNameDesc", "A simple node to demonstrate how to create new MetaSound nodes in C++. Adds two floats together"),
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
				using namespace GetNoteNumFromName;

				FDataReferenceCollection InputDataReferences;

				// InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputAValue), InputA);
				// InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputBValue), InputB);
                InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputWaveAssetName), WaveAsset);

				return InputDataReferences;
			}

			// Allows MetaSound graph to interact with your node's outputs
			virtual FDataReferenceCollection GetOutputs() const override
			{
				using namespace GetNoteNumFromName;

				FDataReferenceCollection OutputDataReferences;

				OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutputNoteNumberName), OutputNoteNumber);

				return OutputDataReferences;
			}

			// Used to instantiate a new runtime instance of your node
			static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
			{
				using namespace GetNoteNumFromName;

				const Metasound::FDataReferenceCollection& WaveAssetollection = InParams.InputDataReferences;
				const Metasound::FInputVertexInterface& InputInterface = DeclareVertexInterface().GetInputInterface();

				// TDataReadReference<float> InputA = WaveAssetollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputAValue), InParams.OperatorSettings);
				// TDataReadReference<float> InputB = WaveAssetollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InputBValue), InParams.OperatorSettings);
                TDataReadReference<FWaveAsset> WaveAsset = WaveAssetollection.GetDataReadReferenceOrConstructWithVertexDefault<FWaveAsset>(InputInterface, METASOUND_GET_PARAM_NAME(InputWaveAssetName), InParams.OperatorSettings);
				return MakeUnique<FGetNoteNumFromNameOperator>(WaveAsset);
			}

			// Primary node functionality
			void Execute()
			{
				CurrentWaveAsset = *WaveAsset;
				FSoundWaveProxyPtr WaveProxy = CurrentWaveAsset.GetSoundWaveProxy();
				if(WaveProxy.IsValid())
				{
					FString First = GetLastChars(*CurrentWaveAsset->GetFName().ToString(), 1);
					FString Second = GetLastChars(*CurrentWaveAsset->GetFName().ToString(), 2);
					*OutputNoteNumber = CovertToMIDINum(First, Second);
				}
			}

	private:

		// Inputs
		// FFloatReadRef InputA;
		// FFloatReadRef InputB;
        FWaveAssetReadRef WaveAsset;
		FWaveAsset CurrentWaveAsset;

		// Outputs
		FFloatWriteRef OutputNoteNumber;

		FString GetLastChars(FString InString, int ReversedIndex)
		{
			int lens = InString.Len();
			if(lens<2 || (ReversedIndex<1||ReversedIndex>2))
				return InString;
			else
			{	
				if(ReversedIndex == 1)
				{
					return InString.Mid(lens-1,1);
				}
				else
				{
					return InString.Mid(lens-2,1);
				}	
			}
		}

		float CovertToMIDINum(FString first, FString second)
		{
			//C3 is middle note
			//C-2 = 0
			//C-1 = 12
			//alphabet+12*(n+2)

			if(first.IsNumeric()&&!second.IsNumeric())
			{
				float degreeNum = (FCString::Atof(*first)+2)*12;
				float noteNum = AlphabetToNum(second);
				return degreeNum + noteNum;
			}
			else if(second.IsNumeric()&&!first.IsNumeric())
			{
				float degreeNum = (FCString::Atof(*second)+2)*12;
				float noteNum = AlphabetToNum(first);
				return degreeNum + noteNum;
			}
			else
				return -1;
			
		}
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
	class FGetNoteNumFromName : public FNodeFacade
	{
		public:
			FGetNoteNumFromName(const FNodeInitData& InitData)
				: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FGetNoteNumFromNameOperator>())
			{
			}
	};

	// Register node
	METASOUND_REGISTER_NODE(FGetNoteNumFromName);
}
#undef LOCTEXT_NAMESPACE